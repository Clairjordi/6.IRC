#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "lib.h"

class Client
{
	private:
		Client();
		const int		_socket;
		bool			_password;
		string			_nickname;
		string			_username;
		int				_chanSubs;
		bool			_quit;

	public:
		string			_waitCmd;

		//Constructor and Destructor
		Client(int const &);
		~Client();
		
		//Getter

		bool 			getPassword() const;
		string const &	getNickname() const;
		string const &	getUsername() const;
		int const & 	getSocket() const;
		int				getChanSubs() const;
		bool 			getQuit() const;

		//Setter
		void			setPassword(bool);
		void			setNickname(string);
		void			setUsername(string);
		void			setQuit(bool);

		//Functions
		void 			addChanSubs();
		void 			removeChanSubs();
};

#endif
