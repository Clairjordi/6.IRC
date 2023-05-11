#include "Channel.hpp"

// Constructors and Destructors
Channel::Channel(string name, string topic, string key): _name(name), _topic(topic), _key(key)
{
	_inviteOnly = false;
	_topicSettable = false;
	_limitNbUsers = 0;
}

Channel::~Channel()
{}

//Getter - Setter
string const &Channel::getChanName() const
{
	return _name;
}

string const &Channel::getChanTopic() const
{
	return _topic;
}

string const &Channel::getChanKey() const
{
	return _key;
}

bool	Channel::getInviteOnly() const
{
	return _inviteOnly;
}

string Channel::getChanUsers()
{
	_usersList = "";
	for (vector<string>::iterator it = _chanUsers.begin(); it != _chanUsers.end(); ++it)
	{
		if (it != _chanUsers.begin())
			_usersList += ", ";
		_usersList += (*it);
	}
	return _usersList;
}

vector<string> Channel::getVectorChanUsers() const
{
	return _chanUsers;
}

bool	Channel::getSettableTopic() const
{
	return _topicSettable;
}

unsigned int	Channel::getUserLimit() const
{
	return _limitNbUsers;
}

void	Channel::setChanTopic(string chanTopic)
{
	_topic = chanTopic;
}

void	Channel::setChanKey(string chanKey)
{
	_key = chanKey;
}

void	Channel::setInviteOnly(bool invite)
{
	_inviteOnly = invite;
}

void Channel::setTopic(string topic)
{
	_topic = topic;
}

void Channel::settableTopic(bool settable)
{
	_topicSettable = settable;
}

void Channel::setUserLimit(unsigned int limit)
{
	_limitNbUsers = limit;
}

//Functions
void	Channel::removeChanUser(string chanUser)
{
	vector<string>::iterator it = find(_chanUsers.begin(), _chanUsers.end(), chanUser);
	if (it != _chanUsers.end())
		_chanUsers.erase(it);
	else
		throw logic_error("User not a member of this channel");
}

void	Channel::removeChanOp(string op)
{
	vector<string>::iterator it = find(_chanOp.begin(), _chanOp.end(), op);
	if (it != _chanOp.end())
		_chanOp.erase(it);
	else
		return;
}

void	Channel::removeInviteUser(string invite)
{
	vector<string>::iterator it = find(_inviteUsers.begin(), _inviteUsers.end(), invite);
	if (it != _inviteUsers.end())
		_inviteUsers.erase(it);
	else
		return;
}

void	Channel::addChanUser(string chanUser)
{
	_chanUsers.push_back(chanUser);
}

void	Channel::addInviteUser(string invite)
{
	_inviteUsers.push_back(invite);
}

void Channel::addOpToChan(string user)
{
	_chanOp.push_back(user);
}

bool	Channel::checkLimitNbUsers()
{
	if (_chanUsers.size() == _limitNbUsers)
		return true;
	return false;
}

bool Channel::checkRegisteredUser(string user)
{
    vector<string>::iterator users = find(_chanUsers.begin(), _chanUsers.end(), user);
	if (users != _chanUsers.end())
       	return true;
    return false;
}

bool Channel::checkOpUser(string user)
{
    vector<string>::iterator users = find(_chanOp.begin(), _chanOp.end(), user);
	if (users != _chanOp.end())
       	return true;
    return false;
}

vector<string>::iterator Channel::getInviteUsers(string user)
{
	vector<string>::iterator it = find(_inviteUsers.begin(), _inviteUsers.end(), user);
	if (it != _inviteUsers.end())
    	return it;
	else
		throw logic_error("User not invited in this invite-only channel");
	return it;
}
