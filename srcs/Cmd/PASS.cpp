#include "Command.hpp"

// Set the password for the connection with server
// PASS <pwd>
void Command::pass(Client& client, vector<string> args)
{
	if (client.getPassword() == true)
		throw logic_error("You are already registered");
	if (args.size() == 2 && args[1] == _pwd)
	{
        client.setPassword(true);
		string message = "Password is good, now please finish your registration with : NICK <nickname> and USER <username>\r\n";
		if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "send failed !" << endl;
	}
	else
        throw logic_error("Put password : PASS <password>");
}
