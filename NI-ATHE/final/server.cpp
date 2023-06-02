#include <cstdio>

#include "CServer.h"
#include "CSocketServer.h"

void ParsePort(int argc, char ** argv, int & port) {
    if (argc < 2) {
        port = Constants::DEFAULT_SERVER_PORT;
        return;
    }
    std::stringstream ss (argv[1]);
    if (!(ss >> port))
        port = Constants::DEFAULT_SERVER_PORT;
}

int main(int argc, char ** argv) {
    int port;
    ParsePort(argc, argv, port);
    try {
        std::unique_ptr<CSocket> socket = std::make_unique<CSocketServer>(port);
        auto & socketServer = dynamic_cast<CSocketServer &>(*socket);
        printf("Starting server...\n");
        socketServer.Start([] (const CSocket & socket) {
            printf("%d: Connected.\n", socket.Id());
            return CServer(socket).RunThread();
        });
        printf("Server started on port %d\n", socketServer.Port());

        printf("Write anything or press Ctrl+D to stop:\n");
        scanf("%*c");

        printf("Stopping server...\n");
        socketServer.Stop();
        printf("Server stopped.\n");
        return 0;
    }
    catch (const SocketException & e) {
        printf("%s\n", e.what());
        return 1;
    }
}
