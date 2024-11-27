#ifndef SERVER_H
#define SERVER_H

#include <vector>

int handle_message(int socket, const std::vector<char> message);

#endif /*SERVER_H*/