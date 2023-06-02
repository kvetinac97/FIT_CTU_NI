#pragma once

#include "CMessage.h"

class CMessageChannelChange: public CMessage {
public:
    CMessageChannelChange() : CMessage(1) {}
    bool ProcessMessage(std::map<CSocket, CUser> & clients, std::mutex & clientMtx,
                        CSocket & socket, const std::vector<std::string> & messages) const override {
        std::string channel = messages[0];
        if (channel.empty() || channel[0] != '#') // bad channel
            return socket.SendMessage(message_type_e::MSG_TYPE_ACKNOWLEDGE_JOIN, {channel, "false"});

        {
            std::unique_lock<std::mutex> ul (clientMtx);
            auto it = clients.find(socket);
            it->second.SetChannel(channel);
        }
        return socket.SendMessage(message_type_e::MSG_TYPE_ACKNOWLEDGE_JOIN, {channel, "true"});
    }
};
