#include "Server.hpp"
#include "Command.hpp"

static bool signals = false;

// Change bool signals who stop the server.
static void signal_handler_server(int sig)
{
	if (sig == SIGINT)
	{
		signals = true;
		cout << "\nShutdown server" << endl;
	}
}

//Check params's programm
void	Server::checkArgv(char **argv)
{
	if (argv[2][0] == '\0')
			throw logic_error("Password is empty");

	for (int i = 0; argv[1][i]; i++)
	{
		if (isdigit(argv[1][i]) == 0)
			throw logic_error("Wrong port, only digit required");
	}
	int port = atoi(argv[1]);
	if (port < 6665 || port > 6669)
		throw logic_error("Wrong port, port range is 6665-6669");

	for (int i = 0; argv[2][i]; i++)
	{
		if (isgraph(argv[2][i]) == 0)
			throw logic_error("Wrong password");
	}
}

// Create the server and parse the input.
Server::Server(char **argv)
{
	checkArgv(argv);
	signal(SIGINT, signal_handler_server);
	_command.setPwd(argv[2]); 
	initialize(argv[1]);
}

// initialize the server, he's ready to be launch
void	Server::initialize(char *port)
{
	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketServer == ERROR)
		throw runtime_error("Failed to create socket");
	int opt = true;
	if(setsockopt(_socketServer, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == ERROR)
		throw runtime_error("Failed to set option to socket");
	struct sockaddr_in addrServer;
	bzero(&addrServer, sizeof(addrServer));
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(atoi(port));
	if (bind(_socketServer, (struct sockaddr *)&addrServer, sizeof(addrServer)) == ERROR)
		throw runtime_error("Failed to associate socket");
	if (listen(_socketServer, 10) == ERROR)
		throw runtime_error("Failed to connect to socket");
}

// Close all socket before the destruction server.
Server::~Server()
{
	map<int, Client>::iterator it;

	for (it = _clientSocket.begin(); it != _clientSocket.end(); ++it)
		close(it->first);
	close(_socketServer);
}

// Add a client connect to the server.
void Server::addClient()
{
	int		addrlen = 0;
	char    *message = (char *)"Welcome to IRCserv !!!\r\n"; 
	int     new_socket;
	struct sockaddr_in address;

	new_socket = accept(_socketServer, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if (new_socket == ERROR)
	{
		cerr << "Accept failed" << endl;
		return ;
	}
	
	cout << "New connection" << endl;
	cout << "Socket fd : " << new_socket << "\n" << endl;

	if(send(new_socket, message, strlen(message), 0) == ERROR)
		cerr << "Send failed" << endl;

	Client c(new_socket);
	_clientSocket.insert(pair<int,Client>(new_socket, c));
}

// Remove a client connexion from the server.
void Server::removeClient(map<int, Client>::iterator it)
{
	cout << "Host disconnected" << endl;
	cout << "Socket fd : " << it->first << "\n" << endl;

	close(it->first);
	_clientSocket.erase(it);
}

// Launch the server, he's ready to be use.
void    Server::launch()
{
	fd_set  fds;
	int     activity,
			valread,
			max_sd;
	char buffer[512];
	map<int, Client>::iterator it;

	while(!signals)
	{
		FD_ZERO(&fds);
		FD_SET(_socketServer, &fds);
		max_sd = _socketServer;

		for (it = _clientSocket.begin(); it != _clientSocket.end(); ++it)
		{
			FD_SET(it->first , &fds);
			if(it->first > max_sd)
				max_sd = it->first;
    	}

		activity = select(max_sd + 1 , &fds , NULL , NULL , NULL);
		if (!signals && activity == ERROR)
			cerr << "Select error" << endl;
		if (signals)
			break ;
		if (FD_ISSET(_socketServer, &fds))
			addClient();
		for (it = _clientSocket.begin(); it != _clientSocket.end(); ++it)
		{  
			if (FD_ISSET(it->first , &fds))
			{
				bzero(&buffer, sizeof(buffer));
				valread = recv(it->first, buffer, sizeof(buffer), 0);
				if (valread == ERROR)
					cerr << "Recv error" << endl;
				else if (valread > 0)
				{
					_command.handleCommand(_clientSocket, it->second, buffer);
					if (it->second.getQuit() == true)
						removeClient(it);
				}
				else
				{
					if (!it->second.getUsername().empty())
						_command.handleCommand(_clientSocket, it->second, "QUIT\r\n");
					removeClient(it);
				}
				break;
			}
		}
	}
}
