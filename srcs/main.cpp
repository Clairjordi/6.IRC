#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cerr << "Wrong format : must be ./ircserv <port> <password>" << endl;
        return (1);
    }
	try
	{		
		Server serv(argv);
		serv.launch();
	}
	catch (const exception &e)
	{
		cerr << "Error: " << e.what() << endl;
	}
    return (0);
}

