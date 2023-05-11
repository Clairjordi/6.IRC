#include "Command.hpp"

//PART without arguments leaves all channels in which the user is located
void Command::partFromAll(Client &client)
{
	if (_chan.empty())
		throw logic_error("No channel on this server");
	string message;
	bool member = false;
	for (vector<Channel>::iterator it = _chan.begin(); it != _chan.end(); ++it)
	{
		if (it->checkRegisteredUser(client.getNickname()) == true)
		{
			if (it->checkOpUser(client.getNickname()) == true)
				it->removeChanOp(client.getNickname());
			it->removeChanUser(client.getNickname());
			client.removeChanSubs();
			message = client.getNickname() + " left the channel " + it->getChanName() + ".\r\n";
			if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
				cerr << "Send failed" << endl;
			sendAllUsersChannel(client, it, message);
			if (checkEmptyChannel(it))
				it--;
			if (member == false)
				member = true;
		}
	}
	if (member == false)
		 throw logic_error("User isn't part of any other known channels");
}

//To exit a particular channel without message
void Command::partFromOne(Client &client, vector<string> args)
{
	if (args[1][0] != '#')
		throw logic_error("Not a channel name");
	vector<Channel>::iterator it = findChannel(args[1]);
	if (it == _chan.end())
		throw logic_error("Channel not found");
	else
	{
		if (it->checkRegisteredUser(client.getNickname()) == true)
		{
			if (it->checkOpUser(client.getNickname()) == true)
				it->removeChanOp(client.getNickname());
			it->removeChanUser(client.getNickname());
			client.removeChanSubs();
			string message = client.getNickname() + " left the channel " + it->getChanName() + ".\r\n";
			if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
				cerr << "Send failed" << endl;
			sendAllUsersChannel(client, it, message);
		}
		else
			throw logic_error("User isn't part of this channel");
	}
	checkEmptyChannel(it);
}

//To exit a particular channel or channels with or without a reason message 
void Command::partFromMultiple(Client &client, vector<string> params)
{
	string message;
	
	if (params[params.size() - 1].find(':') != string::npos)
	{
		if (checkPosReason(params) == false)
			throw logic_error(INVALID_REASON);
	}
	for (size_t i = 1; i < params.size(); i++)
	{
		if (params[i][0] == ':' && i == params.size() - 1)
			break ;
		vector<Channel>::iterator it = findChannel(params[i]);
		if (it->checkOpUser(client.getNickname()) == true)
			it->removeChanOp(client.getNickname());
		it->removeChanUser(client.getNickname());
		client.removeChanSubs();
		if (params[params.size() - 1].find(':') == string::npos)
			message = client.getNickname() + " left the channel " + it->getChanName() + "\r\n";
		else 
			message = client.getNickname() + " left the channel " + it->getChanName() + " because " + params[params.size() - 1] + "\r\n";
		if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		sendAllUsersChannel(client, it, message);
		if (checkEmptyChannel(it))
			it--;
	}
}

//To exit a a one or more channel(s)
// PART
// PART <channel>
// PART <channel> :message
// PART <channel>,<channel> :message
// PART <channel> <channel> 
// PART <channel> <channel> :message 
void Command::part(Client &client, vector<string> args)
{
	for (size_t i = 1; i < args.size(); i++)
	{
		if (isCmd(args[i]))
			throw logic_error(NO_CMD_NAME);
	}
	vector<string> params = checkParams(args);
	if (args.size() == 1)
		partFromAll(client);
	else if (params.size() == 2)
		partFromOne(client, params);
	else							
		partFromMultiple(client, params);
}
