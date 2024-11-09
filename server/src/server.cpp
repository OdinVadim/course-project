#include <iostream>

//Подключаем заголовочный файл с реализацией взаимодействия клиента и сервера
#include "connection.h"

#ifdef WIN
#include "h.h"
#endif /*WIN*/

using namespace std;

int main()
{

#ifdef WIN
    cout << "It's server " << kafedra << "\n";
#endif /*WIN*/

    return 0;
}