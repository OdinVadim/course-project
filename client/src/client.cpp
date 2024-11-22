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
        send_message(client_socket, input);

        std::string message = "";
        while (message.length() == 0)
        {
            recieve_message(client_socket, message);
        }
        cout << "[Server] " << message << "\n";

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