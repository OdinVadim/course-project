#include "message.h"

#include <iostream>

#include "connection.h"
#include "sorts.h"
#include "search.h"

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

int message_echo(int socket, const std::vector<char> message)
{
    //Выводим сообщение от клиента
    std::cout << "[Client " << socket << "] ";
    for (int i = 1; i < message.size(); i++)
    {
        std::cout << message[i];
    }
    std::cout << "\n";

    //Создаём "эхо"
    std::vector<char> echo = std::vector<char>(message.size() - 1);
    for (int i = 0; i < message.size() - 1; i++)
    {
        echo[i] = message[i+1];
    }

    //Отправляем сообщение клиенту
    if (send_message(socket, echo) < 0)
    {
        return -1;
    }

    return 0;
}

int message_sort(int socket, const std::vector<char> message)
{
    //Проверяем корректность сообщения
    if (message.size() % int_size != 1)
    {
        std::cout << "[Info] Client " << socket << " send uncoorrect data\n";
        return 0;
    }

    //Размер массива чисел
    int arr_size = message.size() / int_size;

    //Создаём массив чисел arr
    std::vector<int> arr = std::vector<int>(arr_size);

    //Записваем в arr данные из message
    for (int i = 0; i < arr_size; i++)
    {
        unsigned char a1 = (message[i*int_size + 1]);
        unsigned char a2 = (message[i*int_size + 2]);
        unsigned char a3 = (message[i*int_size + 3]);
        unsigned char a4 = (message[i*int_size + 4]);

        arr[i] = (a1 << 24) | (a2 << 16) | (a3 << 8) | a4;
    }

    //Сортировка выбором
    if (message[0] == char(server_command::selection_sort))
    {
        std::cout << "[Client " << socket << "] Requested selection sorting\n";
        selection_sort(arr);
        std::cout << "[Info] Selection sorting has been done\n";
    }
    //Сортировка Шелла
    else if (message[0] == char(server_command::shell_sort))
    {
        std::cout << "[Client " << socket << "] Requested shell sorting\n";
        shell_sort(arr);
        std::cout << "[Info] Shell sorting has been done\n";
    }
    //Некорректные данные
    else
    {
        std::cout << "[Info] Client " << socket << " send uncoorrect data\n";
        return 0;
    }

    //Создаём массив sort, который будет отправлен обратно клиенту
    std::vector<char> sort = std::vector<char>(arr_size*int_size);
    
    //Записываем данные из arr в sort
    for (int i = 0; i < arr_size; i++)
    {
        sort[i*4] = (arr[i] & 0xFF00'0000) >> 24;
        sort[i*4 + 1] = (arr[i] & 0x00FF'0000) >> 16;
        sort[i*4 + 2] = (arr[i] & 0x0000'FF00) >> 8;
        sort[i*4 + 3] = (arr[i] & 0x0000'00FF);
    }

    //Отправляем данные обратно клиенту
    if (send_message(socket, sort) < 0)
    {
        return -1;
    }

    return 0;
}

int handle_message(int socket, const std::vector<char> message)
{
    //Отключение сервера
    if (message[0] == char(server_command::exit))
    {
        std::cout << "[Info] Recieve exit message from client " << socket << "\n";
        return 1;
    }
    //Эхо
    else if (message[0] == char(server_command::echo))
    {
        if (message_echo(socket, message) < 0)
        {
            return -1;
        }
    }
    //Сортировка массива
    else if ((message[0] & 0b0010'0000) != 0)
    {
        if (message_sort(socket, message) < 0)
        {
            return -1;
        }
    }
    else
    {
        std::cout << "[Info] Client " << socket << " send uncoorrect data\n";
    }

    return 0;
}