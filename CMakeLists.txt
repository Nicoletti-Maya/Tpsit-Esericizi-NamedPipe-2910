cmake_minimum_required(VERSION 4.2)
project(NamedPipe C)

set(CMAKE_C_STANDARD 11)

add_executable(Ping Ping.c)
add_executable(Pong Pong.c)

add_executable(EsercizioB EsercizioB.c)
add_executable(EsercizioB2 EsercizioB2.c)

add_executable(Es1client Es1client.c)
add_executable(Es1server Es1server.c)

add_executable(Es2client Es2client.c)
add_executable(Es2server Es2server.c)

add_executable(Es3chata Es3chata.c)
add_executable(Es3chatb Es3chatb.c)

add_executable(EsercizioA EsercizioA.c)
