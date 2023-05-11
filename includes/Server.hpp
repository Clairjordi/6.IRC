#ifndef SERVER_HPP
#define SERVER_HPP

# include "lib.h"
# include "Client.hpp"
# include "Command.hpp"

class Server
{
	private:
		Server();

		int                 _socketServer;
		map<int, Client>    _clientSocket;
		Command             _command;
		
		void				checkArgv(char **argv);
		void				initialize(char *);
		void    			addClient();
		void    			removeClient(map<int, Client>::iterator);

	public:
	//Constructor and Destructor
		Server(char **);
		~Server();
	
	//Functions
		void    			launch();
};

#endif
