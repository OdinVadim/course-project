#include "command.h"

#include <iostream>
#include <vector>

#include "connection.h"

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

void command_exit(int socket)
{
    std::vector<char> message = { char(server_command::exit) };
    send_message(socket, message);

    return;
}

int command_echo(int socket)
{
    //Ввод сообщения от пользователя
    std::string input;
    std::cin >> input;

    //Создаём vector<char> message
    std::vector<char> message = std::vector<char>(input.length() + 1);

    //Записываем сообщение в message
    message[0] = char(server_command::echo);
    for (int i = 0; i < input.length(); i++)
    {
        message[i + 1] = input[i];
    }

    //Отправляем сообщение на сервер
    if (send_message(socket, message) < 0)
    {
        return -1;
    }

    //Очищаем сообщение и принимаем его с сервера
    message.clear();
    if (recieve_message(socket, message) < 0)
    {
        return -1;
    }

    //Выводим полученное сообщение
    std::cout << "[Server] ";
    for (int i = 0; i < message.size(); i++)
    {
        std::cout << message[i];
    }
    std::cout << "\n";

    return 0;
}

int handle_command(int socket, const std::string& command)
{
    //Выключение клиента
    if (command == "close")
    {
        return -1;
    }
    //Выключение сервера
    else if (command == "exit")
    {
        command_exit(socket);
        return -1;
    }
    //Эхо
    else if (command == "echo")
    {
        return command_echo(socket);
    }
    else
    {
        std::cout << "[Client] " << command << " is unknown command\n";
    }

    return 0;
}