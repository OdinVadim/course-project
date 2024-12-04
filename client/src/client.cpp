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
        std::string input;
        std::cin >> input;

        std::vector<char> send = std::vector<char>(input.length());

        for (int i = 0; i < input.length(); i++)
        {
            send[i] = input[i];
        }

        send_message(socket, send);

        std::vector<char> message;
        recieve_message(socket, message);

        std::cout << "[Server] ";

        int message_length = (message[0] << 24) + (message[1] << 16) + (message[2] << 8) + (message[3]);

        for (int i = 0; i < message_length - 4; i++)
        {
            std::cout << message[i + 4];
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