#include "message.h"

#include <iostream>

#include "connection.h"
#include "sorts.h"
#include "search.h"

//Список команд сервера
enum class server_command
{
    exit = 0,
    echo = 1,

    linear_search = 0b0001'0000,
    sentinel_linear_search = 0b0001'0001,
    binary_search = 0b0001'0010,
    linear_search_with_sort = 0b0001'0011,

    selection_sort = 0b0010'0000,
    shell_sort = 0b0010'0001
};

int handle_message(int socket, const std::vector<char> message)
{
    std::cout << "[Client: " << socket << "] ";

    int message_length = (message[0] << 24) + (message[1] << 16) + (message[2] << 8) + (message[3]);

    for (int i = 0; i < message_length - 4; i++)
    {
        std::cout << message[i + 4];
    }
    std::cout << "\n";

    if ((message.size() == 1) && (message[0] == char(server_command::exit)))
    {
        std::cout << "[Info] Recieve exit message from client " << socket << "\n";

        return 1;
    }

    send_message(socket, message);

    return 0;
}