 #include "Command.hpp"

void Command::kickFromChannel(Client &client, vector<string> args, vector<Channel>::iterator it)
{
	(void) client;
    vector<string> params = checkParams(args);
	if (args.size() == 4 && (checkPosReason(params) == false))
		throw logic_error(INVALID_REASON);
	string message;
    for (size_t i = 2; i < params.size(); i++)
    {
		char colon = ':';
		if (params[i].find(colon) == string::npos)
		{
			if (it->checkRegisteredUser(params[i]) == false)
				throw logic_error("User " + params[i] + " isn't registered in channel " + it->getChanName());
			findClient(params[i])->second.removeChanSubs();
			message = params[i] + " have been kicked from channel " + it->getChanName() + catchReason(params) + "!\r\n";
			sendAllUsersChannel(findClient(params[i])->second, it, message);
			message = "You have been kicked from channel " + it->getChanName() + catchReason(params) + "!\r\n";
			if (send(findClient(params[i])->first, message.c_str(), message.size(), 0) == ERROR)
				cerr << "Send failed" << endl;
			it->removeChanUser(params[i]);
			if (checkEmptyChannel(it))
				it--;
		}
		else
			break ;
	}
}

//KICK #channel nickname1,nickname2,nickname3,...,nicknameN :reason
//Kick someone from the channel and send a notification to all user's channel
void Command::kick(Client& client, vector<string> args)
{
	if (args.size() < 3)
		throw logic_error("Need between one and three parameters : <channel> <nickname> [<reason>]");
	for (size_t i = 1; i < args.size(); i++)
	{
		if (isCmd(args[i]))
			throw logic_error(NO_CMD_NAME);
	}
	vector<Channel>::iterator it = findChannel(args[1]);
	if (it->getChanName() == args[1] && it->checkOpUser(client.getNickname()) == false)
		throw logic_error("You must be an operator member of the channel in order to kick someone");
	kickFromChannel(client, args, it);
}
