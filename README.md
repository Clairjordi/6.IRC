# IRC

Projet Ecole 42: (en groupe)

Server : ```./ircser <port 6665-6669> <keyword>```                                                                                                                                                         
Client : cd Client/ ```./sic -h <localhost> -p <port> [-n <nickname>] [-k <keyword>]```

Recréer un tchat IRC [Internet Relay Chat : protocole TCP/IP de communication par message sur internet] avec les commandes de bases suivantes :

**ALL USERS**
- PASS password
- NICK nickname
- USER username
- JOIN #channel *key*
- TOPIC #channel *:topic*
- PRIVMSG receiver :text_to_be_sent
- PART #channel *:reason*
- LIST *channel*
- QUIT *:message*

**OPERATORS'S CHANNEL**
- KICK #channel nickname *:comment*
- INVITE nickname #channel
- TOPIC #channel *:topic*
- MODE #channel +|- i|t|k|o|l *limit* *user*

>	_Various information_
- Operator's channel only :
	- INVITE : Send an invitation to someone to enter a specific channel
	- KICK : Kick someone from the channel and send a notification to all user's channel
	- MODE : Change the channel’s mode by an operator
		- i : Set/remove Invite-only channel
		- t : Set/remove the restrictions of the TOPIC command to channel operators
		- k : Set/remove the channel key (password)
		- o : Give/take channel operator privilege
		- l : Set/remove the user limit to channel

- All users :
	- PASS : Set the password for the connection with server
	- NICK: Set the Nickname
	- USER : Set the Username		
	- JOIN : Create or join a channel(s)
	- TOPIC : Display or set a specific channel topic
	- PART : To exit a a one or more channel(s)
	- LIST : List all the channels or the member's channel if the channel is specified
	- PRIVMSG : Send a message to a user(s) or a channel(s)  
	- QUIT : Quit the server with or without a reason

## Protocole IRC
- Le protocole IRC utilise un système de serveurs et de clients pour permettre aux utilisateurs de se connecter à des canaux de discussion en temps réel.
- Les clients IRC peuvent se connecter à un serveur IRC en utilisant un nom d'utilisateur et un mot de passe. Les utilisateurs peuvent rejoindre des canaux de discussion ou discuter en privé avec d'autres utilisateurs connectés au même serveur.
- Les commandes IRC sont utilisées pour exécuter des actions, telles que rejoindre un canal, quitter un canal, envoyer un message privé, etc.
- Les canaux IRC sont des espaces de discussion en temps réel où les utilisateurs peuvent discuter en groupe.
- Les opérateurs de canal IRC (ou "ops") sont des utilisateurs qui ont des privilèges spéciaux sur un canal, tels que l'expulsion d'autres utilisateurs ou la modification des paramètres du canal.
