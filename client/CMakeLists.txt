#Название подпроекта
project(client)

#Инициализация переменной SRC, содержащей в себе список всех исходных файлов клиента, которые должны лежать в папке src
file(GLOB SRC "src/*.cpp")

#Указание директории включаемых файлов
include_directories(include)
#Указание директории, в которую будет записан исполняемый файл
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

#При сборке под системы Windows определяем макрос WIN
if (WIN32)
    add_compile_definitions(WIN)
endif()
#При сборке под системы UNIX определяем макрос UNIX
if (UNIX)
    add_compile_definitions(UNIX)
endif()

#Сборка исполняемого файла клиента. Первый аргумент - название проекта, второй аргумент - список всех компилируемых файлов.
add_executable(client ${SRC})