#include "Command.hpp"

//check that the parameters are not commands
bool Command::isCmd(string str)
{
  map<string, void(Command::*)(Client&, vector<string>)>::iterator it;
  for(it = _allCmd.begin(); it != _allCmd.end(); it++)
  {
    if (it->first == str)
		return true;
  }
  return false;
}

//check for nickname or username if already existed
bool Command::alreadyExisting(string str, char c)
{
	map<int, Client>::iterator it;
	for (it = _clients->begin(); it != _clients->end(); it++)
	{
		if ((c == 'n' && it->second.getNickname() == str)
			|| (c == 'u' && it->second.getUsername() == str))
			return false;
	}
	return true;
}

//check nickname and username respect rules
bool Command::check_param(string str)
{
	int i;

	i = 0;
	if (str.size() > 9 || str.size() < 1)
		return false;
	while (str[i])
	{
		if (!isalnum(str[i]))
		{
			if (str[i] != '-' &&
			str[i] != '_' &&
			str[i] != '[' &&
			str[i] != ']' &&
			str[i] != '{' &&
			str[i] != '}' &&
			str[i] != '\\' &&
			str[i] != '`' &&
			str[i] != '^' &&
			str[i] != '|')
				return false;
		}
		i++;
	}
	return true;
}

//Split for the function checkParams
void Command::splitParams(string const & str, const char delim, vector<string> & out)
{
    stringstream ss(str);
    string s;

	while (getline(ss, s, delim)) {
        out.push_back(s);
    }
}

//split list of channels or users or channel,key separated by commas
vector<string> Command::checkParams(vector<string> args)
{
	vector<string> params;
	for (vector<string>::iterator it = args.begin(); it != args.end(); ++it)
	{
		if ((*it).find(':') != string::npos)
		{
			params.push_back(*it);
			break ;
		}
        splitParams(*it, ',', params);
	}
	return (params);
}

//returns an iterator on the corresponding channel
vector<Channel>::iterator Command::findChannel(string channel)
{
	vector<Channel>::iterator it;
	for (it = _chan.begin(); it != _chan.end(); it++)
	{
			if (it->getChanName() == channel)
				break;
	}
	if (it == _chan.end())
		throw logic_error("Channel " + channel + " not found");
	return (it);
}

//returns an iterator on the corresponding user
map<int, Client>::iterator Command::findClient(string user)
{
	map<int, Client>::iterator it;
	for (it = _clients->begin(); it != _clients->end(); ++it)
	{
		if (it->second.getNickname() == user)
			break;
	}
	if (it == _clients->end())
		throw logic_error("User " + user + " isn't on this server");
	return (it);
}

//check that the reason is written correctly
bool Command::checkPosReason(vector<string> params)
{
	bool colon = false;
	for (size_t i = 1; i < params.size(); i++)
	{
		for (size_t j = 0; j < params[i].size(); j++)
		{
			if (colon == true && params[i][j] == ':' )
				return false;
			if (colon == false && j == 0 && params[i][j] == ':')
			{
				if (params[i].size() == 1)
					return false;
				if (isspace(params[i][j + 1]) != 0)
					return false;
				colon = true;
			}
		}
	}
	return colon;
}

// Get the reason in the vector
string Command::catchReason(vector<string> arg)
{
	string msg;

	for(size_t i = 0; i < arg.size(); i++)
	{
		if (arg[i][0] == ':')
		{
			msg = arg[i];
			break ;
		}
	}
	return (msg);
}

//send a message to all users of channel
void Command::sendAllUsersChannel(Client & client, vector<Channel>::iterator it, string message)
{
	vector<string> users = it->getVectorChanUsers(); 
	vector<string>::iterator it_chanUsers;
	map<int, Client>::iterator it_clients;
		for (it_chanUsers = users.begin(); it_chanUsers != users.end(); it_chanUsers++)
	{
		for (it_clients = _clients->begin(); it_clients != _clients->end(); it_clients++)
		{
			if (*it_chanUsers == client.getNickname())
				break ;
			if (*it_chanUsers == it_clients->second.getNickname())
			{
				if (send(it_clients->first, message.c_str(), message.size(), MSG_NOSIGNAL) == ERROR)
					cerr << "Send failed" << endl;
				break ;
			}
		}
	}
}

//check charaters password's channel are correct
bool Command::checkKey(string key)
{
	for (int i = 0; key[i]; i++)
	{
		if (isprint(key[i]) == 0)
			return false;
		if (key[i] == '\n' || key[i] == '\r' || key[i] == 32
			|| key[i] == ',' || key[i] == ';' || key[i] == ':'
			|| key[i] == '(' || key[i] == ')' || key[i] == '['
			|| key[i] == ']' || key[i] == '{' || key[i] == '}'
			|| key[i] == '<' || key[i] == '>' || key[i] == '"'
			|| key[i] == '+' || key[i] == '-' || key[i] == '\'')
			return false;
	}
	return true;
}

//check if the channel no longer has user and delete it
bool Command::checkEmptyChannel(vector<Channel>::iterator chan)
{
	if (chan->getVectorChanUsers().empty())
	{
		_chan.erase(chan);
		return true;
	}
	return false;
}
