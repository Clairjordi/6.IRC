#include "Command.hpp"

//put or remove the mode invite only (i)
void Command::inviteOnlyMode(Client &client, vector<string> args, vector<Channel>::iterator channel)
{
	if (args.size() != 3)
			throw logic_error("If you want to set this channel in invite-only, please type: MODE <channel> <+/-i>");
	if (args[2][0] == '-')
	{
		channel->setInviteOnly(false);
		string message = "Channel " + channel->getChanName() + " is no longer in Invite-Only mode\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " unsets the Invite-Only mode of channel" + channel->getChanName() + "\r\n";
		sendAllUsersChannel(client, channel, message);
	}
	else
	{
		channel->setInviteOnly(true);
		string message = "Channel " + channel->getChanName() + " is in Invite-Only mode\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " unset the Invite-Only mode of channel" + channel->getChanName() + "\r\n";
		sendAllUsersChannel(client, channel, message);
	}
}

//allowed to modified a topic channel (t)
void Command::topicMode(Client &client, vector<string> args, vector<Channel>::iterator channel)
{
	if (args.size() != 3)
			throw logic_error("If you want to set the topic mode, please type: MODE <channel> <+/-t>");
	if (args[2][0] == '-')
	{
		channel->settableTopic(false);
		string message = "Channel " + channel->getChanName() + "'s topic can be modified by all users channel\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " let the topic of channel " + channel->getChanName() + "editable to all registered members" + "\r\n";
		sendAllUsersChannel(client, channel, message);
	}
	else
	{
		channel->settableTopic(true);
		string message = "Channel " + channel->getChanName() + "'s topic can be modified by operators only\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " let the topic channel " + channel->getChanName() + " editable only to operators" + "\r\n";
		sendAllUsersChannel(client, channel, message);
	}
}

//put or remove a channel's password (k)
void Command::keyMode(Client &client, vector<string> args, vector<Channel>::iterator channel)
{
	if (args[2][0] == '+')
	{
		if (args.size() != 4)
			throw logic_error("If you want to set a password for this channel, please type: MODE <channel> <+k> <password>");
		if (checkKey(args[3]) == false)
			throw logic_error("Wrong key format");
		channel->setChanKey(args[3]);
		string message = "Channel " + channel->getChanName() + "'s key has been updated : " + channel->getChanKey() + "\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " changed the key's channel " + channel->getChanName() + " to " + args[3] + "\r\n";
		sendAllUsersChannel(client, channel, message);
	}
	else
	{
		channel->setChanKey("");
		string message = "Channel " + channel->getChanName() + " no longer requires a key\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " unset the key's channel " + channel->getChanName() + "\r\n";
		sendAllUsersChannel(client, channel, message);
	}
}

//put or remove operator rights for a user (o)
void Command::operatorMode(Client &client, vector<string> args, vector<Channel>::iterator channel)
{
	if (args.size() != 4)
		throw logic_error("If you want to modify operator status to a specific user in this channel, please type: MODE <channel> <+/-o> <nickname>");
	if (channel->checkRegisteredUser(args[3]) == false)
			throw logic_error("The user is not in this channel");
	string message;
	if (args[2][0] == '+')
	{
		if (channel->checkOpUser(args[3]) == true)
			throw logic_error("The user is already registered as an operator");
		channel->addOpToChan(args[3]);
		message = args[3] + " is operator for " + channel->getChanName() + " channel\r\n";
		if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " put you in operator's channel " + channel->getChanName() + " channel\r\n";
		sendAllUsersChannel(client, channel, message);
	}
	else
	{
		if (channel->checkOpUser(args[3]) == false)
			throw logic_error("The user is already registered as an user only");
		if (args[3] != client.getNickname())
		{
			message =  args[3] + " isn't anymore an operator of " + channel->getChanName() + " channel\r\n";
			if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
				cerr << "Send failed" << endl;
		}
		message = client.getNickname() + " removed you from the channel operators " + channel->getChanName() + " channel\r\n";
		sendAllUsersChannel(client, channel, message);
		channel->removeChanOp(args[3]);
	}
}

//set or unset a limit user option for a specific channel (l)
void Command::userLimitMode(Client &client, vector<string> args, vector<Channel>::iterator channel)
{
	if (args[2][0] == '-')
	{	
		if (args.size() != 3)
			throw logic_error("If you want to set limit user for this channel, please type: MODE <channel> <-l>");
		channel->setUserLimit(0);
		string message = "Channel " + channel->getChanName() + " doesn't have a limited number of user\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " unset the limit of users in channel " + channel->getChanName() + "\r\n";
		sendAllUsersChannel(client, channel, message);
	}
	else
	{
		if (args.size() < 4)
			throw logic_error("If you want to specify a limit of user in this channel, please type: MODE <channel> <+l> <max_user>");
		for(string::iterator it = args[3].begin(); it != args[3].end(); ++it)
		{
			if (!isdigit(*it))
				throw logic_error("User limit need to be a positive number");
		}
		channel->setUserLimit(atoi(args[3].c_str()));
		string message = "Channel " + channel->getChanName() + " has a limit of " + args[3] + " users\r\n";
    	if (send(client.getSocket(), message.c_str(), message.size(), 0) == ERROR)
			cerr << "Send failed" << endl;
		message = client.getNickname() + " sets a limited number of users in channel " + channel->getChanName() + " to " + args[3] + "\r\n";
		sendAllUsersChannel(client, channel, message);
	}
}

//check is option is written correctly
static void checkOpt(string option)
{
	if (option[0] != '-' && option[0] != '+')
		throw logic_error("Option for MODE command must start with '-' or '+'");
	if (option.length() != 2)
		throw logic_error("Option format must start with '-' or '+' and followed by a letter between i; t; k; o; l");
}

//MODE allows you to set or remove options for a specific channel
//syntax, see below 
void Command::mode(Client& client, vector<string> args)
{
	if (args.size() < 3)
		throw logic_error("Need two parameters : <channel> <option : i; t; k; o; l>");
	for (size_t i = 1; i < args.size(); i++)
	{
		if (isCmd(args[i]))
			throw logic_error(NO_CMD_NAME);
	}

	vector<Channel>::iterator channel = findChannel(args[1]);
	if (channel->getChanName() == args[1] && channel->checkOpUser(client.getNickname()) == false)
		throw logic_error("You must be an operator in order to set the channel's mode");
	checkOpt(args[2]);
	switch (args[2][1])
	{
		case 'i':
			inviteOnlyMode(client, args, channel);
			break;
		case 't':
			topicMode(client, args, channel);
			break;
		case 'k':
			keyMode(client, args, channel);
			break;
		case 'o':
			operatorMode(client, args, channel);
			break;
		case 'l':
			userLimitMode(client, args, channel);
			break;
		default:
			throw logic_error("Available options for MODE are : i; t; k; o; l");
			break;
	}
}

// i : invite-only
// MODE <channel> +i
// deactivate option :
// MODE <channel> -i
// 
// t : editable topic 
// MODE <channel> +t
// deactivate option :
// MODE <channel> -t
// 
// k : key
// MODE <channel> +k <pwd>
// deactivate option :
// MODE <channel> -k
// 
// o : operator
// MODE <channel> +o <nickname> 
// // deactivate option :
// MODE <channel> -o <nickname>
// 
// l : limiter nb users
// MODE <channel> +l <nb-max-users>
// deactivate option :
// MODE <channel> -l
