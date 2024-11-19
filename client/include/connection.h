#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <string>

//Подключение к серверу
int connect_to_server();
//Отключение от сервера
void disconnect_from_server(int client_socket);

//Обработка подключения
int handle_connection(int client_socket);

//Отправка сообщение серверу
int send_message(const std::string& message);
//Приём сообщения от сервера
int recieve_message(std::string& message);

//Создание сокета
int create_socket();

#endif //CLIENT_CONNECTION_H