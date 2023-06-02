#include <iostream>
#include <memory>

#include "CSocketClient.h"

using namespace std;

bool ReadUsername(string & username) {
    printf("Write your username:\n");
    return getline(cin, username) && !username.empty();
}

void ParsePort(int argc, char ** argv, std::string & hostname, int & port) {
    if (argc < 3) {
        hostname = Constants::DEFAULT_HOSTNAME;
        port = Constants::DEFAULT_SERVER_PORT;
        return;
    }
    hostname = argv[1];
    std::stringstream ss (argv[2]);
    if (!(ss >> port))
        port = Constants::DEFAULT_SERVER_PORT;
}

int main(int argc, char ** argv) {
    string username, hostname;
    int port;
    if (!ReadUsername(username)) {
        printf("Error: Could not read username\n");
        return 1;
    }
    ParsePort(argc, argv, hostname, port);
    try {
        unique_ptr<CSocket> socket = make_unique<CSocketClient>();
        auto & client = dynamic_cast<CSocketClient &>(*socket);
        printf("Connecting to server at %s:%d...\n", hostname.c_str(), port);
        while (!client.Connect(hostname.c_str(), port)) {
            printf("Could not connect: retrying in 5 seconds...\n");
            client.Reset();
            sleep(5);
        }
        printf("Connected!\n");

        if (!client.SetUsername(username) || !client.JoinChannel("#public")) {
            printf("Failed to boot. Shutting down now...\n");
            close(client.Id());
            return 1;
        }
        client.Start();
    }
    catch (const SocketException & e) {
        printf("%s\n", e.what());
        return 1;
    }
    return 0;
}
