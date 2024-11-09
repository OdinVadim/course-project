#include "connection.h"

//Подключаем заголовочный файл, который предоставляет реализацию сокетов на Unix-системах
#ifdef UNIX
#include <sys/socket.h>
#endif /*UNIX*/