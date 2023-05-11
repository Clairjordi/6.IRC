#include "Command.hpp"

//set or replace the nickname
//NICK <nickname>
void Command::nick(Client& client, vector<string> args)
{
	if (args.size() != 2)
		throw logic_error("Need 1 parameter : <nickname>");
	if (isCmd(args[1]))
		throw logic_error(NO_CMD_NAME);
	if (!alreadyExisting(args[1], 'n'))
		throw logic_error(ALREADY_EXISTS);
	if(!check_param(args[1]))
		throw logic_error("Wrong nickename\nNickename have 9 character\nCharacter allowed : digit, letter, -, _, [], {}, \\, `, |, ^");
	client.setNickname(args[1]);
	string message = "You set your nickname to " + client.getNickname() + "\r\n";
	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
		cerr << "Send failed" << endl;
}
