#include "Command.hpp"

//Check if the channel name is spelled correctly
static bool checkName(const string& nameChan)
{
	if (nameChan.size() < 2)
		return false;
    if (nameChan[0] != '#' || nameChan.size() > MAX_CHAN_NAME)
        return false;
    for (size_t i = 0; i < nameChan.size(); i++)
    {   
		if (nameChan[i] == '\n' || nameChan[i] == '\r' || nameChan[i] == 32
			|| nameChan[i] == ',' || nameChan[i] == ';' || nameChan[i] == ':'
			|| nameChan[i] == '(' || nameChan[i] == ')' || nameChan[i] == '['
			|| nameChan[i] == ']' || nameChan[i] == '{' || nameChan[i] == '}'
			|| nameChan[i] == '<' || nameChan[i] == '>' || nameChan[i] == '"'
			|| nameChan[i] == '\'' || nameChan[i] == '\0')
			return false;
        if (!isprint(nameChan[i]))
			return false;
    }
    return true;
}

//Checks if it's an existing channel - specific for the JOIN command
bool Command::channelExists(string &nameChan)
{
	vector<Channel>::iterator it;
	for (it = _chan.begin(); it != _chan.end(); it++)
	{
		if (it->getChanName() == nameChan)
			return true;
	}	
	return false;
}

//Create a channel with or without a keyword - the user who creates the channel is automatically operator
void Command::createChannel(Client &client, vector<string> params, string chanName, size_t i)
{
	if (client.getChanSubs() >= MAX_CHANNEL)
			throw logic_error("You can join only 30 channels");
	Channel channel(chanName, "", "");
    channel.addChanUser(client.getNickname());
    channel.addOpToChan(client.getNickname());
	client.addChanSubs();
	if (i + 1 < params.size() && params[i + 1][0] != '#')
		channel.setChanKey(params[i + 1]);
	_chan.push_back(channel);
	if (channel.getChanTopic().empty())
	{
		string message = "Welcome to the " + params[i] + " channel\nPlease enter topic's channel <" + chanName + "> | Use topic's command : TOPIC <channel> <topic>\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
	}
}

// Add a user to an existing channel
void Command::checkUserForAdd(Client &client, vector<string> params, size_t i)
{
	vector<Channel>::iterator it;
	if (params[i][0] == '#')
	{
		it = findChannel(params[i]);
		if (it->checkRegisteredUser(client.getNickname()))
				throw logic_error("User already registered in Channel " + it->getChanName());
		if (it->checkLimitNbUsers() == true)
			throw logic_error("Channel " + it->getChanName() + " is full");
		if (client.getChanSubs() >= MAX_CHANNEL)
			throw logic_error("You can join only 30 channels");
		if (it->getChanKey().empty() == false && params.size() == 2)
			throw logic_error("You need a key to enter in Channel " + it->getChanName());
		if	(it->getChanKey().empty() == false && params.size() > 2)
		{
			vector<string>::iterator mdp = find(params.begin(), params.end(), it->getChanKey());
			if (mdp == params.end())
	 			throw logic_error("Wrong key for channel " + it->getChanName() +", please try again with : JOIN <channel> <key>");
		}
		if (it->getInviteOnly() == true)
		{
			vector<string>::iterator it1 = it->getInviteUsers(client.getNickname());
			if (*it1 == client.getNickname())
				it->removeInviteUser(client.getNickname());
		}
		it->addChanUser(client.getNickname());
		client.addChanSubs();
		string message = "Welcome to the " + params[i] + " channel\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " has joined channel " + params[i] + "\r\n";
		sendAllUsersChannel(client, it, message);
	}
}

//Create or join a channel(s) 
// JOIN <channel> <key> 
// JOIN <channel>,<key> 
// JOIN <channel>
// JOIN <channel> <channel>
// JOIN <channel>,<key> <channel> <key> 
// JOIN <channel> <channel>,<key> <channel>
void Command::join(Client &client, vector<string> args)
{
	if (args.size() < 2)
        throw logic_error("Need at least 1 parameter: <channel> [<key>]");
	vector<string> params = checkParams(args);
	if (params.size() == 2 && params[1][0] != '#')
		throw logic_error("Invalid channel name");
	for (size_t i = 1; i < params.size(); i++)
    {
        if (((i > 1 && params[i - 1][0] != '#') || params[i][0] == '#') && !checkName(params[i]))
            throw logic_error("Invalid channel name");
		if (isCmd(params[i]) == true)
			throw logic_error(NO_CMD_NAME);
		if (i % 2 == 0 && params[i][0] != '#' && checkKey(params[i]) == false)
			throw logic_error("Invalid key channel");
		for (vector<Channel>::iterator it = _chan.begin(); it != _chan.end(); it++)
		{
			if (it->getChanName() == params[i] && it->checkRegisteredUser(client.getNickname()))
				throw logic_error("User already registered");
		}
    }
	for (size_t i = 1; i < params.size(); i++)
	{
		string chanName = params[i];
		bool created = false;
		if (channelExists(chanName) == true)
			created = true;
		if (created == false && params[i][0] == '#')
		{
			createChannel(client, params, chanName, i);
			if (i + 1 < params.size() && params[i + 1][0] != '#')
				i++;
		}
		else
			checkUserForAdd(client, params, i);
	}
}
