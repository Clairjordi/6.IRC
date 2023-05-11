# Color messages
GREEN = /bin/echo -e "\x1b[1;3;32m$1\x1b[0m"

# Executable name
NAME = ircserv

#Compilation options
CXX			= c++
CXXFLAGS 	= -Wall -Wextra -Werror -g -I $(INCDIR) -std=c++98

# Directories
INCDIR    = includes
SRCDIR    = srcs
OBJDIR    = obj

# Sources files
SRC_NAME =  main.cpp \
			Server.cpp \
			Client.cpp \
			Command.cpp \
			Channel.cpp \
			Cmd/PASS.cpp \
			Cmd/NICK.cpp \
			Cmd/USER.cpp \
			Cmd/JOIN.cpp \
			Cmd/TOPIC.cpp \
			Cmd/LIST.cpp \
			Cmd/PRIVMSG.cpp \
			Cmd/QUIT.cpp \
			Cmd/PART.cpp \
			Cmd/INVITE.cpp \
			Cmd/KICK.cpp \
			Cmd/MODE.cpp \
			Cmd/utils.cpp \

OBJS = $(addprefix $(OBJDIR)/, $(SRC_NAME:.cpp=.o))

# Rules
$(OBJDIR)/%.o:    $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@


all:		$(NAME)

$(NAME):	$(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $@
		$(call GREEN,"Compilation success !")

clean:

		@if [ -d $(OBJDIR) ]; then \
		rm -rf $(OBJDIR) && \
		$(call GREEN,"Obj cleaned up !"); \
		fi

fclean:		clean
		@if [ -e $(NAME) ]; then \
        rm -rf $(NAME) && \
		$(call GREEN,"Exec cleaned up !"); \
		fi

re:		fclean all

.SILENT:
.PHONY:		all clean fclean re
