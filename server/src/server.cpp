#include <iostream>

//Подключаем заголовочный файл с реализацией взаимодействия клиента и сервера
#include "connection.h"

#ifdef WIN
#include "h.h"
#endif /*WIN*/

using namespace std;

int main()
{
#ifdef UNIX

    int server_soket = start_server();

    while (handle_connection(server_soket, "exit") == 0);

    shutdown_server(server_soket);

#endif /*UNIX*/
#ifdef WIN
    cout << "It's server " << kafedra << "\n";
#endif /*WIN*/

    return 0;
}