#include <iostream>

//Подключаем заголовочный файл с реализацией взаимодействия клиента и сервера
#include "connection.h"

using namespace std;

int main()
{
#ifdef UNIX

    int client_socket = connect_to_server();

    if (client_socket < 0)
    {
        return -1;
    }
    
    std::string input;

    std::cin >> input;

    while (input != "close")
    {
        int length = input.length() + 4;

        std::vector<char> send = std::vector<char>(length);

        send[0] = (length & 0xFF'00'00'00) >> 24;
        send[1] = (length & 0x00'FF'00'00) >> 16;
        send[2] = (length & 0x00'00'FF'00) >> 8;
        send[3] = (length & 0x00'00'00'FF);

        for (int i = 0; i < (length - 4); i++)
        {
            send[i+4] = input[i];
        }

        send_message(client_socket, send);

        std::vector<char> message;
        recieve_message(client_socket, message);

        std::cout << "[Server] ";

        int message_length = (message[0] << 24) + (message[1] << 16) + (message[2] << 8) + (message[3]);

        for (int i = 0; i < message_length - 4; i++)
        {
            std::cout << message[i + 4];
        }
        std::cout << "\n";

        std::cin >> input;
    }

    disconnect_from_server(client_socket);

#endif /*UNIX*/
#ifdef WIN
    cout << "It's client\n";
    cout << "On Windows\n";
#endif /*WIN*/

    return 0;
}