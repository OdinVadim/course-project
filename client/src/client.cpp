#include <iostream>

//Подключаем заголовочный файл с реализацией взаимодействия клиента и сервера
#include "connection.h"

using namespace std;

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
        std::vector<char> exit = { 'e', 'x', 'i', 't' };
        send_message(socket, exit);

        return -1;
    }
    //Эхо
    else if (command == "echo")
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
        send_message(socket, message);

        //Очищаем сообщение и принимаем его с сервера
        message.clear();
        recieve_message(socket, message);

        //Выводим полученное сообщение
        std::cout << "[Server] ";
        for (int i = 0; i < message.size(); i++)
        {
            std::cout << message[i];
        }
        std::cout << "\n";
    }
    else
    {
        cout << "[Client] " << command << " is unknown command\n";
    }

    return 0;
}

int main()
{
    int client_socket = connect_to_server();

    if (client_socket < 0)
    {
        return -1;
    }
    
    std::string command;
    std::cin >> command;

    while (handle_command(client_socket, command) == 0)
    {
        std::cin >> command;
    }

    disconnect_from_server(client_socket);
    return 0;
}