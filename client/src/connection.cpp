#include "connection.h"

#ifdef WIN

//Подключаем заголовочные файлы, которые необходимы для взаимодействия с сокетами на Windows
#include <winsock2.h>
#include <ws2tcpip.h>

//А надо?
#pragma comment(lib, "ws2_32.lib")

//Определяем закрытие функции на Windows
#define CLOSE(s) closesocket(s)
//Определяем проверку сокета на Windows
#define IS_VALID(s) (s != INVALID_SOCKET)

#endif /*WIN*/

#ifdef UNIX

//Подключаем заголовочные файлы, которые необходимы для взаимодействия с сокетами на Unix-системах
#include <sys/socket.h> 
#include <netdb.h>
#include <unistd.h>

//Определяем закрытие функции на UNIX-системах
#define CLOSE(s) close(s)
//Определяем проверку сокета на UNIX-системах
#define IS_VALID(s) (s >= 0)

#endif /*UNIX*/

#include <iostream>
//Файл, содержащий функцию memset()
#include <cstring>

//IP-адрес и номер порта
const char* ip_address = "127.0.0.1";
const char* port = "8080";

//Максимальный размер передаваемого пакета
const int package_length = 16384;

int connect_to_server()
{
    std::cout << "[Info] Connecting to server...\n";

    #ifdef WIN

    //Инициализация Windows Socket API
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        std::cout << "[Error] Failed to initialize WinSockAPI\n";
        return -1;
    }

    #endif /*WIN*/

    //Создаём сокет клиента
    int client_socket = create_socket();

    //Проверяем созданный сокет клиента
    if (!IS_VALID(client_socket))
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
    CLOSE(client_socket);
    std::cout << "[Info] Disconnected from server\n";

    return;
}

int send_message(int client_socket, const std::vector<char>& message)
{
    //Создаём буфер, размер которого равен максимальноиу размеру одного пакета
    char* buffer = new char[package_length];
     //Создаём копию указателя на первый элемент буфера
    char* buffer_ = buffer;

    //Длина передаваемого сообщения
    unsigned int message_length = message.size();

    int a = message_length / package_length;
    int b = message_length % package_length;

    //Отправляем полностью заполненные пакеты
    for (int i = 0; i < a; i++)
    {
        //Записываем пакет из message в буфер
        for (int j = 0; j < package_length; j++)
        {
            *(buffer++) = message[i*package_length + j];
        }

        //Возвращаемся к первому элементу буфера
        buffer = buffer_;

        //Отправляем данные и проверяем, что приём данных был завершён без ошибок
        if (send(client_socket, buffer, package_length, 0) < 0)
        {
            std::cout << "[Error] Failed to send data to server\n";
            //Удаляем буфер
            delete[] buffer;
            return -1;
        }
    }

    //Записываем неполный пакет из message в буфер
    for (int i = 0; i < b; i++)
    {
        *(buffer++) = message[package_length*a + i];
    }

    //Возвращаемся к первому элементу буфера
    buffer = buffer_;

    //Отправляем данные и проверяем, что приём данных был завершён без ошибок
    if (send(client_socket, buffer, b, 0) < 0)
    {
        std::cout << "[Error] Failed to send data to server\n";
        //Удаляем буфер
        delete[] buffer;
        return -1;
    }

    //Удаляем буфер
    delete[] buffer;
    return 0;
}
int recieve_message(int client_socket, std::vector<char>& message)
{
    //Создаём буфер, размер которого равен максимальному размеру одного пакета
    char* buffer = new char[package_length];
    //Создаём копию указателя на первый элемент буфера
    char* buffer_ = buffer;

    //Принимаем первый пакет
    int recieve = recv(client_socket, buffer, package_length, 0);

    //Проверяем, что сервер прислал ненулевые данные
    if (recieve == 0)
    {
        //Удаляем буфер
        delete[] buffer;
        return -1;
    }
    //Проверяем, что приём данных был завершён без ошибок
    if (recieve < 0)
    {
        std::cout << "[Error] Failed recieve data from server\n";
        //Удаляем буфер
        delete[] buffer;
        return -1;
    }

    //Считываем длину передаваемого сообщения
    unsigned int message_length = (*(buffer++) << 24) + (*(buffer++) << 16) + (*(buffer++) << 8) + *(buffer++);

    //Очищаем message
    message.clear();
    //Создаём новый vector<char>, длина которого равняется длине передаваемого сообщения
    message = std::vector<char>(message_length);

    int a = message_length / package_length;
    int b = message_length % package_length;
    
    //Возвращаемся к первому элементу буфера
    buffer = buffer_;

    //Принимаем полностью заполненные пакеты
    for (int i = 0; i < a; i++)
    {
        //Записываем пакет из буфера в message
        for (int j = 0; j < package_length; j++)
        {
            message[package_length*i + j] = *(buffer++);
        }

        //Возвращаемся к первому элементу буфера
        buffer = buffer_;
        
        //Если (i < (a-1)) или (b != 0), то принимаем следующий пакет
        if ((i < (a-1)) || (b != 0))
        {
            recieve = recv(client_socket, buffer, package_length, 0);
        }

        //Проверяем, что сервер прислал ненулевые данные
        if (recieve == 0)
        {
            //Удаляем буфер
            delete[] buffer;

            return -1;
        }
        //Проверяем, что приём данных был завершён без ошибок
        if (recieve < 0)
        {
            std::cout << "[Error] Failed recieve data from server\n";
            //Удаляем буфер
            delete[] buffer;

            return -1;
        }
    }

    //Записываем последний пакет из буфера в message
    for (int i = 0; i < b; i++)
    {
        message[package_length*a + i] = *(buffer++);
    }
    
    //Возвращаемся к первому элементу буфера
    buffer = buffer_;
    //Удаляем буфер
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
    if (!IS_VALID(client_socket))
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