#pragma once

#include <string>

//Запуск сервера
int start_server();
//Выключение сервера
void shutdown_server(int server_socket);

//Обработка подключения
int handle_connection(int server_socket, const std::string& exit_message);

//Подключить клиента, если есть запрос на подключение к серверу
int connect_client(int server_socket);
//Отключить клиента
int disconnect_client(int server_socket, int client_socket);

//Отправка сообщения клиенту
int send_message(int server_socket, int client_socket, const std::string& message);
//Приём сообщения от клиента
int recieve_message(int server_socket, int client_socket, std::string* message);

//Создание сокета
int create_server_socket();