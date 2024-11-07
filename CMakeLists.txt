#Указание минимальной версии CMake, которая может использоваться для сборки проекта
cmake_minimum_required(VERSION 3.16)

#Указываем используемый стандарт C++, требуем его соблюдать
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Название проекта
project(SearchAndSort LANGUAGES CXX)

#Указание директорий подпроектов
add_subdirectory(client)
add_subdirectory(server)