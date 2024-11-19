#include "connection.h"

//Подключаем заголовочный файл, который предоставляет реализацию сокетов на Unix-системах
#ifdef UNIX

#include <sys/socket.h> 
#include <netdb.h>

#include <unistd.h>

#include <iostream>
//Файл, содержащий функцию memset()
#include <cstring>

//IP-адрес и номер порта
const char* ip_address = "127.0.0.1";
const char* port = "8080";

int connect_to_server()
{
    std::cout << "[Info] Connecting to server...\n";
    //Создаём сокет клиента
    int client_socket = create_socket();

    //Проверяем созданный сокет клиента
    if (client_socket < 0)
    {
        return -1;
    }

    std::cout << "[Info] Connected to server\n";

    return 0;
}
void disconnect_from_server(int client_socket)
{
    std::cout << "[Info] Disconnecting from server...\n";
    //Закрываем сокет
    close(client_socket);
    std::cout << "[Info] Disconnected from server\n";

    return;
}

int handle_connection(int client_socket)
{
    return 0;
}

int send_message(int client_socket, const std::string& message)
{
    if (send(client_socket, message.data(), message.size(), 0) < 0)
    {
        std::cout << "[Error] Failed to send data to client " << client_socket << "\n";
        return -1;
    }

    return 0;
}
int recieve_message(int client_socket, std::string& message)
{
    const int length = 128;
    char* buffer = new char[length];

    if (recv(client_socket, buffer, length, 0) < 0)
    {
        std::cout << "[Error] Failed recieve data from client " << client_socket << "\n";
        delete[] buffer;
        return -1;
    }

    message = std::string(buffer);

    delete[] buffer;
    return 0;
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
    //Используем тип сокета TCP
    hints.ai_socktype = SOCK_STREAM;
    //Используем протокол TCP
    hints.ai_protocol = IPPROTO_TCP;

    //Записываем адрес по указателю adress, если функция возвращает не ноль, значит произошла ошибка    
    if (getaddrinfo(ip_address, port, &hints, &address) != 0)
    {
        std::cout << "[Error] Failed to resolve address\n";
        return -1;
    }

    //Создаём сокет
    int client_socket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);

    //Проверяем созданный сокет
    if (client_socket < 0)
    {
        std::cout << "[Error] Failed to create server_socket\n";

        //Освобождаем память от address
        freeaddrinfo(address);
        return -1;
    }

    //Подключаемся к серверу по адресу
    if (connect(client_socket, address->ai_addr, address->ai_addrlen) == -1)
    {
        std::cout << "[Error] Failed to connect to server\n";
        freeaddrinfo(address);
        return -1;
    }

    //Освобождаем память от address
    freeaddrinfo(address);

    return client_socket;
}

#endif /*UNIX*/