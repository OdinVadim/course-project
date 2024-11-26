#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <string>
#include <vector>

//Запуск сервера
int start_server();
//Выключение сервера
void shutdown_server(int server_socket);

//Обработка подключения
int handle_connection(int server_socket, fd_set* socket_polling_list, int& max_socket, const std::string& exit_message);

//Подключить клиента, если есть запрос на подключение к серверу
int connect_client(int server_socket, fd_set* socket_polling_list, int& max_soxket);
//Отключить клиента
void disconnect_client(int client_socket, fd_set* socket_polling_list);

//Отправка сообщения клиенту
int send_message(int client_socket, const std::string& message);
//Приём сообщения от клиента
int recieve_message(std::vector<char>& message, int client_socket);

//Создание сокета
int create_server_socket();

#endif //SERVER_CONNECTION_H