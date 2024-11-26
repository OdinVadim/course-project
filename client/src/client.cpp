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
        char* send_c = new char[length];

        send_c[0] = (length & 0xFF'00'00'00) >> 24;
        send_c[1] = (length & 0x00'FF'00'00) >> 16;
        send_c[2] = (length & 0x00'00'FF'00) >> 8;
        send_c[3] = (length & 0x00'00'00'FF);

        for (int i = 0; i < input.length(); i++)
        {
            send_c[i+4] = input[i];
        }

        std::string send = std::string(send_c, input.length() + 4);

        send_message(client_socket, send);

        std::vector<char> message;
        recieve_message(client_socket, message);

        cout << "[Server] " << (message.data() + 4) << "\n";

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