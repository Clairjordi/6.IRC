#include "Command.hpp"

//Check if the user is an operator
bool Command::checkOpChan(Client &client, vector<string> args, vector<Channel>::iterator it)
{
	if (it->getChanName() == args[1] && it->checkOpUser(client.getNickname()) == false)
		return false;
	return true;
}

//set the topic, topic filled or not 
void Command::setTopic(Client &client, vector<string> args, vector<Channel>::iterator it)
{
	if (it->getSettableTopic() == true && !checkOpChan(client, args, it))
		throw logic_error("You must be an operator channel in order to set the topic's channel");
	it->setChanTopic(args[2]);
	displayTopic(client, it);	
}

//display the topic 
void Command::displayTopic(Client &client, vector<Channel>::iterator it)
{
	if (it != _chan.end())
	{
		string message = "Topic of channel " + it->getChanName() + " " + it->getChanTopic() + "\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
        	cerr << "Send failed" << endl;
	}
}

//if topic is not register, ask for adding one
void Command::askTopic(Client &client, vector<string> args, vector<Channel>::iterator it)
{
	if (it != _chan.end())
	{
		string message = "Channel " + args[1] + " topic is not specified. Please fill it with TOPIC <channel> <topic> as an operator\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
        	cerr << "Send failed" << endl;
	}
}

//Display or set a specific channel topic
//TOPIC <channel> 
//TOPIC <channel> :<topic>
void Command::topic(Client &client, vector<string> args)
{
	if (args.size() < 2 || args.size() > 3)
		throw logic_error("Need 1 or 2 parameters : <channel> [<topic>]");
	for (size_t i = 1; i < args.size(); i++)
	{
		if (isCmd(args[i]))
			throw logic_error(NO_CMD_NAME);
	}
	vector<Channel>::iterator it = findChannel(args[1]);
	if (it == _chan.end())
		throw logic_error("Channel not found");
	if (it->getChanName() == args[1] && it->checkRegisteredUser(client.getNickname()) == false)
		throw logic_error("You must be a registered member of the channel in order to visualize the channel's topic");
	if (args.size() == 3 && !checkPosReason(args))
		throw logic_error(INVALID_REASON);
	if (args.size() == 2)
	{
		if (it->getChanTopic().empty())
			askTopic(client, args, it);
		else
			displayTopic(client, it);
	}
	else
		setTopic(client, args, it);
}
