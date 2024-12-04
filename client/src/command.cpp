#include "command.h"

#include <iostream>
#include <vector>

#include "connection.h"

void command_exit(int socket)
{
    std::vector<char> exit = { 'e', 'x', 'i', 't' };
    send_message(socket, exit);

    return;
}

int command_echo(int socket)
{
    //Ввод сообщения от пользователя
    std::string input;
    std::cin >> input;

    //Записываем сообщение в vector<char>
    std::vector<char> message = std::vector<char>(input.length());
    for (int i = 0; i < input.length(); i++)
    {
        message[i] = input[i];
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