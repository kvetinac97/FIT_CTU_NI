cmake_minimum_required(VERSION 3.24)
project(athens)

set(CMAKE_CXX_STANDARD 17)

add_executable(athens_server server.cpp common.h CSocket.cpp CSocket.h CSocketServer.cpp CSocketServer.h CServer.cpp CServer.h CUser.h messages/CMessage.h messages/CMessageEnd.h messages/CMessageUsername.h messages/CMessageChannelChange.h messages/CMessageChannel.h tp.h)
add_executable(athens_client client.cpp common.h CSocket.cpp CSocket.h CUser.h CSocketClient.cpp CSocketClient.h tp.h)
