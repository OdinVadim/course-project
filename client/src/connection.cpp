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

//Максимальный размер передаваемого пакета
const int package_length = 16383;

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

    return client_socket;
}
void disconnect_from_server(int client_socket)
{
    std::cout << "[Info] Disconnecting from server...\n";
    //Закрываем сокет
    close(client_socket);
    std::cout << "[Info] Disconnected from server\n";

    return;
}

int send_message(int client_socket, const std::vector<char>& message)
{
    char* buffer = new char[package_length];
    char* buffer_ = buffer;

    unsigned int message_length = message.size();

    int a = message_length / package_length;
    int b = message_length % package_length;

    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < package_length; j++)
        {
            *(buffer++) = message[i*package_length + j];
        }

        buffer = buffer_;
        if (send(client_socket, buffer, package_length, 0) < 0)
        {
            std::cout << "[Error] Failed to send data to server\n";
            return -1;
        }
    }

    for (int i = 0; i < b; i++)
    {
        *(buffer++) = message[package_length*a + i];
    }

    buffer = buffer_;
    if (send(client_socket, buffer, b, 0) < 0)
    {
        std::cout << "[Error] Failed to send data to server\n";
        return -1;
    }

    return 0;
}
int recieve_message(int client_socket, std::vector<char>& message)
{
    char* buffer = new char[package_length];
    char* buffer_ = buffer;

    int recieve = recv(client_socket, buffer, package_length, 0);

    if (recieve == 0)
    {
        delete[] buffer;
        return -1;
    }
    if (recieve < 0)
    {
        std::cout << "[Error] Failed recieve data from server\n";
        delete[] buffer;
        return -1;
    }

    unsigned int message_length = (*(buffer++) << 24) + (*(buffer++) << 16) + (*(buffer++) << 8) + *(buffer++);

    message.clear();
    message = std::vector<char>(message_length);

    int a = message_length / package_length;
    int b = message_length % package_length;
    
    buffer = buffer_;

    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < a; j++)
        {
            message[package_length*i + j] = *(buffer++);
        }

        recieve = recv(client_socket, buffer, package_length, 0);

        if (recieve == 0)
        {
            buffer = buffer_;
            delete[] buffer;

            return -1;
        }
        if (recieve < 0)
        {
            std::cout << "[Error] Failed recieve data from server\n";
            buffer = buffer_;
            delete[] buffer;

            return -1;
        }
    }

    for (int i = 0; i < b; i++)
    {
        message[package_length*a + i] = *(buffer++);
    }
    
    buffer = buffer_;
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