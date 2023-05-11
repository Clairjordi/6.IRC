#include "Command.hpp"

//Send a message to a user or a channel  
//PRIVMSG <receiver> :<text> || <receiver>,<receiver> :<text>
void Command::privMsg(Client& client, vector<string> args)
{
	if (args.size() != 3)
		throw logic_error("Need 2 parameters : {<nickname> or <channel>} <text_to_send>");
	for (unsigned int i = 1; i < args.size(); i++)
	{
		if (isCmd(args[i]))
			throw logic_error(NO_CMD_NAME);
	}
	vector<string> params = checkParams(args);
	if (checkPosReason(params) == false)
		throw logic_error(INVALID_REASON);
	string message = "Message from " + client.getNickname() + catchReason(params) + "\r\n";
	map<int, Client>::iterator it_clients;
	vector<Channel>::iterator it_chan;
	string msgchan;
	for (size_t i = 1; i < params.size(); i++)
	{
		if (params[i][0] == '#')
		{
			it_chan = findChannel(params[i]);
			msgchan = "Channel " + it_chan->getChanName() + " "  + message;
			if (it_chan->checkRegisteredUser(client.getNickname()) == false)
				throw logic_error("You aren't a member of this channel so you cannot send message");
			sendAllUsersChannel(client, it_chan, msgchan);
		}
		else if (params[i][0] != ':')
		{
			it_clients = findClient(params[i]);
			if (it_clients->second.getUsername() == "")
				throw logic_error("User isn't registered on this server");

			if (send(it_clients->first, message.c_str(), message.size(), 0) == ERROR)
				cerr << "Send failed" << endl;
		}
		else if (params[i][0] == ':')
			break ;
	}
}
