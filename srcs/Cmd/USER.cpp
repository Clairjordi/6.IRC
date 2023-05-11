#include "Command.hpp"

//Set the username
//USER <username>
void Command::user(Client& client, vector<string> args)
{
	if (args.size() != 2)
		throw logic_error("Need 1 parameter : <username>");
	if (isCmd(args[1]))
		throw logic_error(NO_CMD_NAME);
	if (!alreadyExisting(args[1], 'u'))
		throw logic_error(ALREADY_EXISTS);
	if(!check_param(args[1]))
		throw logic_error("Wrong username\n have 9 character\nCharacter allowed : digit, letter, -, _, [], {}, \\, `, |, ^");
	client.setUsername(args[1]);
	string message = "You set your username to " + client.getUsername() + "\r\n";
	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
		cerr << "Send failed" << endl;
}
