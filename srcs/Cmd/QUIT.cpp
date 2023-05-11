#include "Command.hpp"

// Tell other users that this client has left the server
void Command::leftServMsg(Client & client, vector<string> args)
{
	string message = client.getNickname() + " left the server " + catchReason(args) + "\r\n";

	for (map<int, Client>::iterator it = _clients->begin(); it != _clients->end(); ++it){
		if (client.getNickname() != it->second.getNickname() && !it->second.getUsername().empty()){
			if (send(it->first, message.c_str(), message.size(), MSG_NOSIGNAL) == ERROR)
			 	cerr << "Send failed" << endl;
		}
	}
}

// Remove the user from all registered channels
void Command::removeFromChan(Client &client)
{
	for (vector<Channel>::iterator it = _chan.begin(); it != _chan.end(); ++it){
		if (it->checkRegisteredUser(client.getNickname()) == true){
			it->removeChanUser(client.getNickname());
			client.removeChanSubs();
			string message = client.getNickname() + " left the channel " + it->getChanName() + ".\r\n";
			sendAllUsersChannel(client, it, message);
			if (checkEmptyChannel(it))
				it--;
		}
	}
}

// Quit the server with or without a reason
//QUIT
//QUIT :reason
void Command::quit(Client& client, vector<string> args)
{
	if (args.size() > 2 || (args.size() > 1 && args[1][0] != ':'))
		throw logic_error("Need 1 optional parameter : [<reason>]");
	for (size_t i = 1; i < args.size(); i++)
	{
		if (isCmd(args[i]))
			throw logic_error(NO_CMD_NAME);
	}
	if (args.size() == 2 && checkPosReason(args) == false)
		throw logic_error("Need 1 optional parameter : [<reason>]");
	removeFromChan(client);
	leftServMsg(client, args);
	client.setQuit(true);
}
