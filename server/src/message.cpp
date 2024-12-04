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

    search = 0b0001'0000,

    linear_search = 0b0001'0001,
    sentinel_linear_search = 0b0001'0010,
    binary_search = 0b0001'0011,
    linear_search_with_sort = 0b0001'0100,

    sort = 0b0010'0000,

    selection_sort = 0b0010'0001,
    shell_sort = 0b0010'0010
};

int message_echo(int socket, const std::vector<char> message)
{
    //Выводим сообщение от клиента
    std::cout << "[Client " << socket << "] ";
    for (int i = 1; i < message.size(); i++)
    {
        std::cout << message[i];
    }
    std::cout << "\n";

    //Создаём "эхо"
    std::vector<char> echo = std::vector<char>(message.size() - 1);
    for (int i = 0; i < message.size() - 1; i++)
    {
        echo[i] = message[i+1];
    }

    //Отправляем сообщение клиенту
    if (send_message(socket, echo) < 0)
    {
        return -1;
    }

    return 0;
}

int handle_message(int socket, const std::vector<char> message)
{
    if (message[0] == char(server_command::exit))
    {
        std::cout << "[Info] Recieve exit message from client " << socket << "\n";
        return 1;
    }
    else if (message[0] == char(server_command::echo))
    {
        message_echo(socket, message);
    }
    else
    {
        std::cout << "[Info] Client " << socket << " send uncoorrect data\n";
    }

    return 0;
}