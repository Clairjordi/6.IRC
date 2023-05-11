#ifndef COMMAND_HPP
#define COMMAND_HPP

# include "lib.h"
#include "Channel.hpp"
#include "Client.hpp"

class Command
{
    private:
        map<int, Client>	*_clients;
        string				_pwd;
		vector<Channel>		_chan;

        map<string, void(Command::*)(Client&, vector<string>)> _allCmd;
        void	pass(Client&, vector<string>);
		void	nick(Client&, vector<string>);
		void	user(Client&, vector<string>);
		void	join(Client&, vector<string>);
		void	topic(Client&, vector<string>);
		void	list(Client&, vector<string>);
		void	privMsg(Client&, vector<string>);
		void	quit(Client&, vector<string>);
		void	part(Client&, vector<string>);
		void	invite(Client&, vector<string>);
		void	kick(Client&, vector<string>);
		void	mode(Client&, vector<string>);
       
		//\\ Commands
		//JOIN
		bool 	channelExists(string&);
		bool	checkMaxChanUser(int);
		void	createChannel(Client&, vector<string>, string, size_t);
		void	checkUserForAdd(Client&, vector<string>, size_t);
		//TOPIC
		void	askTopic(Client&, vector<string>, vector<Channel>::iterator);
		void 	displayTopic(Client&, vector<Channel>::iterator);
		void	setTopic(Client&, vector<string>, vector<Channel>::iterator);
		bool	checkOpChan(Client&, vector<string>, vector<Channel>::iterator);
		//KICK
		void	kickFromChannel(Client&, vector<string>, vector<Channel>::iterator);
		//PART
		void	partFromAll(Client&);
		void	partFromOne(Client&, vector<string>);
		void	partFromMultiple(Client&, vector<string>);
		//LIST
		void	listAll(Client&);
		void	listUsers(Client&, vector<string>);
		//INVITE
		void	sendInvitation(map<int, Client>::iterator, vector<Channel>::iterator, Client&);
		//QUIT
		void	removeFromChan(Client&);
		void	leftServMsg(Client&, vector<string>);
		//MODE
		void 	inviteOnlyMode(Client&, vector<string>, vector<Channel>::iterator);
		void 	topicMode(Client&, vector<string>, vector<Channel>::iterator);
		void 	keyMode(Client&, vector<string>, vector<Channel>::iterator);
		void 	operatorMode(Client&, vector<string>, vector<Channel>::iterator);
		void 	userLimitMode(Client&, vector<string>, vector<Channel>::iterator);
		//utils
        bool	isCmd(string);
        bool	check_param(string);
        bool	alreadyExisting(string, char);
		void	splitParams(string const&, const char, vector<string>&);
		vector<string>	checkParams(vector<string>);
		vector<Channel>::iterator	findChannel(string);
		bool	checkPosReason(vector<string>);
		string	catchReason(vector<string> arg);
		void	sendAllUsersChannel(Client&, vector<Channel>::iterator, string);
		map<int, Client>::iterator	findClient(string);
		bool	checkKey(string);
		bool 	checkEmptyChannel(vector<Channel>::iterator);

		void 	initMapCmd();
        void    launchCommand(Client&, string);
        void    checkRegister(Client& client, vector<string> const&);
		void	joinServMsg(Client & client);
    
    public:
    	//Constructor and Destructor
        Command();
        ~Command();
    
    	//Setter
        void	setPwd(string pwd);
    
    	//Functions
        void    handleCommand(map<int, Client> &, Client &, string);
};

#endif
