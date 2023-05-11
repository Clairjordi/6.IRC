#include "Command.hpp"

//Display all existing channels
void Command::listAll(Client &client)
{	
	if (_chan.empty())
		throw logic_error("No channels on this server");
	string channels;
	for (vector<Channel>::iterator it = _chan.begin(); it != _chan.end(); ++it)
	{
		if (it != _chan.begin())
			channels += ", ";
		channels += it->getChanName();
	}
	string message = "Available channels: " + channels + "\r\n";
    if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
       	cerr << "Send failed" << endl;
}

//Display the users of a channel
void Command::listUsers(Client &client, vector<string> args)
{
	vector<Channel>::iterator it = findChannel(args[1]);
	string users = it->getChanUsers();
	string message = "Users of channel " + it->getChanName() + ": " + users + "\r\n";
    if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
    	cerr << "Send failed" << endl;
}

//List all the channels or the member's channel if the channel is specified
// LIST
// LIST <channel>
void Command::list(Client& client, vector<string> args)
{
	if (args.size() > 2)
		throw logic_error("Need a maximum of one parameter : LIST [<channel>]");
	for (size_t i = 1; i < args.size(); i++)
	{
		if (isCmd(args[i]))
			throw logic_error(NO_CMD_NAME);
	}
	if (args.size() == 1)
		listAll(client);
	else
		listUsers(client, args);
}
