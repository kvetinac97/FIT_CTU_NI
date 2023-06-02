#pragma once

#include "CMessage.h"

class CMessageUsername: public CMessage {
public:
    CMessageUsername() : CMessage(1) {}
    bool ProcessMessage(std::map<CSocket, CUser> & clients, std::mutex & clientMtx,
                        CSocket & socket, const std::vector<std::string> & messages) const override {
        std::string username = messages[0];
        {
            std::unique_lock<std::mutex> ul (clientMtx);
            auto it = clients.find(socket);
            auto it2 = std::find_if(clients.begin(), clients.end(), [&](const auto & pair) {
                return pair.second.Username() == username;
            });

            if (it2 != clients.end())
                return socket.SendMessage(message_type_e::MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK, {username, "false"});

            it->second.SetUsername(username);
        }
        return socket.SendMessage(message_type_e::MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK, {username, "true"});
    }
};
