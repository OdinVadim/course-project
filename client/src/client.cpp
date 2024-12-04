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
        std::vector<char> exit = { 0x00, 0x00, 0x00, 0x08, 'e', 'x', 'i', 't' };
        send_message(socket, exit);

        return -1;
    }
    else
    {
        int length = command.length() + 4;

        std::vector<char> send = std::vector<char>(length);

        send[0] = (length & 0xFF'00'00'00) >> 24;
        send[1] = (length & 0x00'FF'00'00) >> 16;
        send[2] = (length & 0x00'00'FF'00) >> 8;
        send[3] = (length & 0x00'00'00'FF);

        for (int i = 0; i < (length - 4); i++)
        {
            send[i+4] = command[i];
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