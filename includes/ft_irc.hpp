#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <string>
# include <sstream>
# include <arpa/inet.h>
# include <functional>
# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
# include <errno.h>
# include <poll.h>
# include <cstdlib>
# include <vector>
# include <map>
# include <algorithm>
# include <csignal>

# include "Clients.hpp"
# include "Server.hpp"
# include "Channels.hpp"

int	strtoint(std::string src);
std::string	inttostr(int src);
int validArgs( std::string port, std::string password );


#endif
