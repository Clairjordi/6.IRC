#include "Client.hpp"

// Constructors and Destructors
Client::Client(int const & socket): _socket(socket), _password(false), _chanSubs(0), _quit(false)
{}

Client::~Client()
{}

//Getter
bool Client::getPassword() const
{
    return _password;
}

string const & Client::getNickname() const
{
    return _nickname;
}

string const & Client::getUsername() const
{
    return _username;
}

int const & Client::getSocket() const
{
    return _socket;
}

int Client::getChanSubs() const
{
	return _chanSubs;
}

bool Client::getQuit() const
{
    return _quit;
}

//Setter
void Client::setPassword(bool pwd)
{
    _password = pwd;
}

void Client::setNickname(string nick)
{
    _nickname = nick;
}

void Client::setUsername(string user)
{
    _username = user;
}

void Client::setQuit(bool quit)
{
    _quit = quit;
}

//Functions
void Client::addChanSubs()
{
    _chanSubs++;
}

void Client::removeChanSubs()
{
	_chanSubs--;
}
