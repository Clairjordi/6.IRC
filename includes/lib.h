#ifndef LIB_H
#define LIB_H

# define ERROR -1

# define UNKNOWN_CMD	"Command not found"
# define NO_CMD_NAME	"Params can not be a command name"
# define ALREADY_EXISTS "This name already exists, please put another one"
# define INVALID_PARAMS	"Params is invalid"
# define INVALID_REASON "Message is badly written"
# define MAX_CHANNEL 30
# define MAX_CHAN_NAME 200

# include <iostream>
# include <signal.h>
# include <unistd.h>
# include <cstdlib>
# include <cstring>
# include <cerrno>
# include <stdexcept>
# include <vector>
# include <map>
# include <algorithm>
# include <sstream>

// lib network programming 
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/time.h>

using namespace std;

#endif
