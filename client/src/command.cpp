#include "command.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "connection.h"

//Размер целого числа в байтах
const int int_size = 4;

//Список команд сервера
enum class server_command
{
    exit = 0,
    echo = 1,

    search = 0b0001'0000,

    linear_search = 0b0001'0001,
    sentinel_linear_search = 0b0001'0010,
    binary_search = 0b0001'0011,
    linear_search_with_sort = 0b0001'0100,

    sort = 0b0010'0000,

    selection_sort = 0b0010'0001,
    shell_sort = 0b0010'0010
};

void command_exit(int socket)
{
    std::vector<char> message = { char(server_command::exit) };
    send_message(socket, message);

    return;
}

int command_echo(int socket)
{
    //Ввод сообщения от пользователя
    std::string input;
    std::cin >> input;

    //Создаём vector<char> message
    std::vector<char> message = std::vector<char>(input.length() + 1);

    //Записываем сообщение в message
    message[0] = char(server_command::echo);
    for (int i = 0; i < input.length(); i++)
    {
        message[i + 1] = input[i];
    }

    //Отправляем сообщение на сервер
    if (send_message(socket, message) < 0)
    {
        return -1;
    }

    //Очищаем сообщение и принимаем его с сервера
    message.clear();
    if (recieve_message(socket, message) < 0)
    {
        return -1;
    }

    //Выводим полученное сообщение
    std::cout << "[Server] ";
    for (int i = 0; i < message.size(); i++)
    {
        std::cout << message[i];
    }
    std::cout << "\n";

    return 0;
}

int command_sort(int socket)
{
    //Ввод пути файла
    std::string file_name;
    std::cin >> file_name;

    //Открываем файл
    std::fstream file;
    file.open(file_name);

    //Проверяем, открылся ли файл
    if(!file.is_open())
    {
        std::cout << "[Error] Failed to open file " << file_name << "\n";
        return 0;
    }

    //Размер файла
    unsigned int file_size = 0;

    //Вычисляем размер файла
    while(!file.eof())
    {
        int b;
        file >> b;

        file_size++;
    }

    //Возвращаемся к началу файла
    file.clear();
    file.seekg(0, std::ios::beg);

    //Создаём сообщение message, которое будет передано на сервер
    std::vector<char> message = std::vector<char>(file_size*int_size + 1);

    //Записываем массив чисел в message
    for (int i = 0; i < file_size; i++)
    {
        int b;
        file >> b;

        message[i*int_size + 1] = (b & 0xFF00'0000) >> 24;
        message[i*int_size + 2] = (b & 0x00FF'0000) >> 16;
        message[i*int_size + 3] = (b & 0x0000'FF00) >> 8;
        message[i*int_size + 4] = (b & 0x0000'00FF);
    }

    std::string method;
    std::cin >> method;

    if (method == "selection")
    {
        message[0] = char(server_command::selection_sort);
    }
    else if (method == "shell")
    {
        message[0] = char(server_command::shell_sort);
    }
    else
    {
        std::cout << "[Client] " << method << " is unknown method\n";
        return 0;
    }

    if (send_message(socket, message) < 0)
    {
        return -1;
    }

    message.clear();
    if (recieve_message(socket, message) < 0)
    {
        return -1;
    }

    for (int i = 0; i < message.size() / int_size; i++)
    {
        std::cout << "[Server] Array:";

        int b;
        b = (message[int_size*i] << 24) + (message[int_size*i + 1] << 16) + (message[int_size*i + 2] << 8) + (message[int_size*i + 3]);

        std::cout << " " << b;
    }
    std::cout << "\n";

    return 0;
}

int handle_command(int socket, const std::string& command)
{
    //Выключение клиента
    if (command == "close")
    {
        return -1;
    }
    //Выключение сервера
    else if (command == "exit")
    {
        command_exit(socket);
        return -1;
    }
    //Эхо
    else if (command == "echo")
    {
        return command_echo(socket);
    }
    //Сортировка
    else if (command == "sort")
    {
        return command_sort(socket);
    }
    //Неизвестная команда
    else
    {
        std::cout << "[Client] " << command << " is unknown command\n";
    }

    return 0;
}