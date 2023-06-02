#pragma once

#include <atomic>
#include <iostream>
#include <iomanip>
#include <map>
#include <netdb.h>
#include <thread>

#include "CSocket.h"

class CSocketClient : public CSocket {
    std::thread m_Receiver;
    std::atomic_bool m_Stopped;

    std::string m_Username, m_Channel;
    void Receiver();
    void ProcessCommand(const std::string &);
public:
    explicit CSocketClient();

    bool Connect(const char * hostname = Constants::DEFAULT_HOSTNAME, int port = Constants::DEFAULT_SERVER_PORT);
    void Reset();

    bool SetUsername (const std::string &);
    bool JoinChannel (const std::string &);
    void Start();
    void Stop() override;
};
