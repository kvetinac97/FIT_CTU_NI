#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "CSocket.h"
#include "CUser.h"

class CSocketServer : public CSocket {
    int m_Port;
    std::thread m_Receiver;
    std::map<int, std::thread> m_Threads;
    std::mutex m_ThreadMtx;

    const int MAX_CONNECTIONS = 10;
    void Receiver(const std::function<void(CSocket)> &);
public:
    explicit CSocketServer(int port = Constants::DEFAULT_SERVER_PORT);
    int Port() const;
    void Start(const std::function<void(CSocket)> &);
    void Stop() override;
};
