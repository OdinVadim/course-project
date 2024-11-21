#include <iostream>
#include <vector>

//Подключаем заголовочный файл с реализацией взаимодействия клиента и сервера
#include "connection.h"
#include "sorts.h"

#ifdef WIN
#include "h.h"
#endif /*WIN*/

using namespace std;

int main()
{
#ifdef UNIX

    int server_socket = start_server();
    int max_socket = server_socket;

    //Список опрашиваемых сокетов
    fd_set socket_polling_list;

    //Очищаем список опрашиваемых сокетов
    FD_ZERO(&socket_polling_list);
    //Добавляем созданный сокет сервера в список опрашиваемых сокетов
    FD_SET(server_socket, &socket_polling_list);

    while (handle_connection(server_socket, &socket_polling_list, max_socket, "exit") == 0);

    shutdown_server(server_socket);

#endif /*UNIX*/
#ifdef WIN
    cout << "It's server " << kafedra << "\n";
#endif /*WIN*/

    return 0;
}