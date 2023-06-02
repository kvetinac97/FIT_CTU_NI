#pragma once

#include <vector>
#include "CMessage.h"

class CMessageChannel: public CMessage {
public:
    CMessageChannel() : CMessage(3) {}
    bool ProcessMessage(std::map<CSocket, CUser> & clients, std::mutex & clientMtx,
                        CSocket & socket, const std::vector<std::string> & messages) const override {
        std::string myUsername = messages[0], myChannel = messages[1], myMessage = messages[2];
        bool isChannel = !myChannel.empty() && myChannel[0] == '#';

        std::vector<std::pair<CSocket, CUser>> socketUsers;
        std::string username, channel;

        {
            std::unique_lock<std::mutex> ul (clientMtx);
            username = clients.at(socket).Username();
            channel = clients.at(socket).Channel();

            // copy users from same channel / direct message with the username
            std::copy_if(clients.begin(), clients.end(), std::back_inserter(socketUsers), [&](const auto & pair) {
                return isChannel ? pair.second.Channel() == myChannel : pair.second.Username() == myChannel;
            });
        }

        if (username.empty())
            return true;
        if (username != myUsername)
            return printf("User tried to spoof username %s\n", myUsername.c_str()), true;
        if (isChannel && channel != myChannel)
            return printf("User tried to spoof channel %s\n", myChannel.c_str()), true;

        std::vector<CSocket> socketIds; // transform to sockets
        std::transform(socketUsers.begin(), socketUsers.end(), std::back_inserter(socketIds), [](const auto & pair) {
            return pair.first;
        });

        std::string msg;
        for (CSocket & userSocket : socketIds)
            userSocket.SendMessage(message_type_e::MSG_TYPE_MESSAGE, {myUsername, myChannel, myMessage});
        return true;
    }
};
