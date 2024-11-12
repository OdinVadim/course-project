#pragma once

#include <string>

int start_server();
void shutdown_server(int socket);

void send_message();
void receive_message();

int create_socket();