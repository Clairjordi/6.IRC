#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "lib.h"

class Channel
{
	private:
		Channel();
		string			_name;
		string			_topic;
		string			_key;
		string			_usersList;
		bool			_inviteOnly;
		unsigned int	_limitNbUsers;
		bool			_topicSettable;
		vector<string>	_chanOp;
		vector<string>	_chanUsers;
		vector<string> 	_inviteUsers;
		
	public:
		//Constructor and Destructor
		Channel(string, string, string);
		~Channel();
		
		//Getter - Setter
		string const &	getChanName() const;
		string const & 	getChanTopic() const;
		string const & 	getChanKey() const;
		bool 			getInviteOnly() const;
		string			getChanUsers();
		vector<string>	getVectorChanUsers() const;
		bool			getSettableTopic() const;
		unsigned int	getUserLimit() const;
	
		void			setChanTopic(string chanTopic);
		void			setChanKey(string chanKey);
		void			setInviteOnly(bool);
		void			setTopic(string);
		void			settableTopic(bool);
		void			setUserLimit(unsigned int);

		//Functions
		void			addChanUser(string);
		void			addInviteUser(string);
		void			removeChanUser(string);
		void			removeChanOp(string);
		void			removeInviteUser(string);
		bool			checkLimitNbUsers();
		void			addOpToChan(string);
		bool			checkRegisteredUser(string);
		bool			checkOpUser(string);
		vector<string>::iterator	getInviteUsers(string);
};

#endif
