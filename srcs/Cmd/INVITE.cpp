#include "Command.hpp"

void Command::sendInvitation(map<int, Client>::iterator guest, vector<Channel>::iterator channel, Client &client)
{
	channel->addInviteUser(guest->second.getNickname());
	string message = client.getNickname() + " send you an invitation to the " + channel->getChanName() + " channel ! Please type JOIN <channel> in order to accept the invitation\r\n";
    if (send(guest->second.getSocket(), message.c_str(), message.size(), 0) == ERROR)
		cerr << "Send failed" << endl;
}

//INVITE <nickname> <channel>
//Send an invitation to someone to enter a specific channel
void Command::invite(Client& client, vector<string> args)
{
	if (args.size() != 3)
		throw logic_error("Need two parameters : <nickname> <channel>");
	for (unsigned int i = 1; i < args.size(); i++)
	{
		if (isCmd(args[i]))
			throw logic_error(NO_CMD_NAME);
	}
	map<int, Client>::iterator guest = findClient(args[1]);
	vector<Channel>::iterator channel = findChannel(args[2]);
	if (guest->second.getUsername() == "")
		throw logic_error("User isn't registered on this server");
	if (channel == _chan.end())
		throw logic_error("Channel not found");
	if (channel->getChanName() == args[2] && channel->checkRegisteredUser(args[1]) == true)
		throw logic_error("User already a member of this channel");
	if (channel->getChanName() == args[2] && channel->checkOpUser(client.getNickname()) == false)
		throw logic_error("You must be an operator's channel in order to invite someone into the channel");
	sendInvitation(guest, channel, client);
}
