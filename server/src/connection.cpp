#include "connection.h"

#ifdef UNIX

//Подключаем заголовочные файлы, которые необходимы для взаимодействия с сокетами на Unix-системах
#include <sys/socket.h> 
#include <netdb.h>

#include <unistd.h>

#include <iostream>
//Файл, содержащий функцию memset()
#include <cstring>

//IP-адрес и номер порта
const char* ip_address = "127.0.0.1";
const char* port = "8080";

//Список опрашиваемых сокетов
fd_set socket_polling_list;

int start_server()
{
    std::cout << "[Info] Starting the server...\n";
    //Создаём сокет
    int socket = create_socket();
    
    //Проверяем созданный сокет
    if (socket < 0)
    {
        return -1;
    }

    //Очищаем список опрашиваемых сокетов
    FD_ZERO(&socket_polling_list);
    //Добавляем созданный сокет в список опрашиваемых сокетов
    FD_SET(socket, &socket_polling_list);

    std::cout << "[Info] The server is start\n";
    return socket;
}

void shutdown_server(int socket)
{
    std::cout << "[Info] Shutting down the server...\n";
    //Закрываем сокет
    close(socket);
    std::cout << "[Info] The server is shut down\n";
}

int create_socket()
{
    //addrinfo хранит в себе информацию об адресе, по которому будет происходить подключение
    //В hints записываются данные о типе подключения, которые потом будут перезаписаны в address
    //adress будет использоваться для записи итогового адреса
    addrinfo hints, *address;
    
    //Очищаем переменную hints c помощью функции memset, чтобы в adress не попало никаких лишних данных
    std::memset(&hints, 0x00, sizeof(addrinfo));
    //Используем IPv4
    hints.ai_family = AF_INET;
    //Используем протокол TCP
    hints.ai_protocol = SOCK_STREAM;

    //Записываем адрес по указателю adress, если функция возвращает не ноль, значит произошла ошибка
    if (getaddrinfo(ip_address, port, &hints, &address) != 0)
    {
        std::cout << "[Error] Failed to resolve address\n";
        return -1;
    }

    //Создаём сокет
    int socket_ = socket(address->ai_family, address->ai_socktype, address->ai_protocol);

    //Проверяем созданный сокет
    if (socket_ < 0)
    {
        std::cout << "[Error] Failed to create socket\n";

        //Освобождаем память от address
        freeaddrinfo(address);
        return -1;
    }

    //Привязываем созданный сокет к адресу
    if (bind(socket_, address->ai_addr, address->ai_addrlen) == -1)
    {
        std::cout << "[Error] Failed to bind socket to address\n";

        //Освобождаем память от address
        freeaddrinfo(address);
        return -1;
    }

    //Освобождаем память от address
    freeaddrinfo(address);

    //Настраиваем сокет
    int yes = 1;
    
    //Настраиваем сокет
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        std::cout << "[Error] Failed to set socket options\n";
        return -1;
    }

    //Настраиваем сокет на прослушивание
    if (listen(socket_, 10) == -1)
    {
        std::cout << "[Error] Failed to activate soket listenner\n";
        return -1;
    }

    //Возвращаем созданный сокет
    return socket_;
}

#endif /*UNIX*/