#include "Command.hpp"

// Initilize the map make pair string / ptr on function
void Command::initMapCmd()
{
    _allCmd["PASS"] = &Command::pass;
    _allCmd["NICK"] = &Command::nick;
	_allCmd["USER"] = &Command::user;
	_allCmd["JOIN"] = &Command::join;
	_allCmd["TOPIC"] = &Command::topic;
	_allCmd["LIST"] = &Command::list;
	_allCmd["PRIVMSG"] = &Command::privMsg;
	_allCmd["QUIT"] = &Command::quit;
	_allCmd["PART"] = &Command::part;
	_allCmd["INVITE"] = &Command::invite;
	_allCmd["KICK"] = &Command::kick;
	_allCmd["MODE"] = &Command::mode;
}

// Constructors and Destructors
Command::Command()
{
	initMapCmd();
}

Command::~Command()
{}

//Setter
void Command::setPwd(string pwd)
{
    _pwd = pwd;
}

//Functions

// Get line of command in vector input
void split(string const &str, const char delim, vector<string> &out)
{
    stringstream ss(str);

    string s;
	string reason = "";

    while (getline(ss, s, delim)) {
		if (s != "")
		{
			if (s[0] == ':')
			{
				reason = s;
				if (getline(ss, s, '\0')) 
					reason += ' ' + s;
                out.push_back(reason);
                break;
			}
			out.push_back(s);
		}
    }
}

// Tell other users that this client has join the server
void Command::joinServMsg(Client &client)
{
	string message = client.getNickname() + " has join the server !" + "\r\n";

	for (map<int, Client>::iterator it = _clients->begin(); it != _clients->end(); ++it){
		if (client.getNickname() != it->second.getNickname() && !it->second.getUsername().empty()){
			if (send(it->first, message.c_str(), message.size(), MSG_NOSIGNAL) == ERROR)
			 	cerr << "Send failed" << endl;
		}
	}
}

// Check if the User has finish to registration : PASS, NICK, USER
void    Command::checkRegister(Client &client, vector<string> const &args)
{
    if (client.getPassword() == false)
    {
        if (args[0] != "PASS")
            throw logic_error("Put password : PASS <password>");
        pass(client, args);
    }
    else if (client.getNickname().empty())
    {
        if (args[0] != "NICK")
            throw logic_error("Put nickname : NICK <nickname>");
        nick(client, args);
    }
    else if (client.getUsername().empty())
    {
        if (args[0] != "USER")
            throw logic_error("Put username : USER <username>");
        user(client, args);
        joinServMsg(client);
    }
}

// Split for execute the command with their params
void Command::launchCommand(Client& client, string str)
{
    vector<string> args;

    split(str, ' ', args);
    if (args.size() < 1)
        throw logic_error(UNKNOWN_CMD);
    if (client.getPassword() == false || client.getNickname().empty() || client.getUsername().empty())
        checkRegister(client, args);
    else
    {
		map<string, void(Command::*)(Client&, vector<string>)>::iterator it;
		it = _allCmd.find(args[0]);
		if (it != _allCmd.end())
			(this->*it->second)(client, args);
		else if (str.find(client.getUsername()) == 0 || str.find(client.getNickname()) == 0)
			checkRegister(client, args);
		else
			throw logic_error("Invalid command");
    }
}

// Remove character \r
void    removeCharacter(string &str)
{
    size_t pos = str.find_last_of('\r');

    while (pos != string::npos)
    {
        str.erase(str.begin() + pos);
        pos = str.find('\r');
    }
}

// check if buffer is complete or Ctrl-D
bool    CompleteCmd(Client &client, string &buffer)
{
    size_t found = buffer.find('\n');

    if (found == string::npos)
    {
        if (buffer[0] != 4)
            client._waitCmd += buffer;
        return false;
    }
    buffer = client._waitCmd + buffer;
    client._waitCmd = "";
    return true;
}


// Function call from server for execute command's line
void    Command::handleCommand(map<int, Client> &clients, Client &client, string buffer)
{
	vector<string> cmd;
    
	_clients = &clients;
    if (!CompleteCmd(client, buffer))
        return;
    removeCharacter(buffer);
    split(buffer, '\n', cmd);
    try
    {
        for (size_t i = 0; i < cmd.size(); i++)
            launchCommand(client, cmd[i]);
    }
    catch (const exception & e)
	{
		string message = "Error: ";
		message	+= e.what();
        message	+= "\r\n";
		if(send(client.getSocket(), message.c_str(),
        	message.size(), 0) == ERROR)
	     	cerr << "send failed !" << endl;
	}
}

