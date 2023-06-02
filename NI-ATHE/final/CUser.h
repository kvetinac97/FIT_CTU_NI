#pragma once

#include <string>

class CUser {
    int m_SocketId;
    std::string m_Channel;
    std::string m_Username;
public:
    CUser (int socketId, std::string channel, std::string username)
    : m_SocketId(socketId), m_Channel(std::move(channel)), m_Username(std::move(username)) {}

    int SocketId() const {
        return m_SocketId;
    }

    const std::string & Channel() const {
        return m_Channel;
    }

    const std::string & Username() const {
        return m_Username;
    }

    void SetUsername (const std::string & username) {
        m_Username = username;
    }

    void SetChannel (const std::string & channel) {
        m_Channel = channel;
    }
};
