#include "CSocketServer.h"

CSocketServer::CSocketServer(int port) : CSocket(), m_Port(port) {
    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind
    if (bind(m_Id, (struct sockaddr *) &address, sizeof(address)) < 0) {
        close(m_Id);
        throw SocketBindException(port);
    }

    // Start listening
    if (listen(m_Id, MAX_CONNECTIONS) < 0) {
        close(m_Id);
        throw SocketPassiveException();
    }

    // Allow reusing address
    int opt = 1;
    setsockopt(m_Id, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
}

int CSocketServer::Port() const {
    return m_Port;
}

void CSocketServer::Receiver(const std::function<void(CSocket)> & serverThread) {
    while (true) {
        sockaddr_in remoteAddress {};
        socklen_t remoteSocketSize;
        int socketId = accept(m_Id, (sockaddr *) &remoteAddress, &remoteSocketSize);
        if (socketId < 0) {
            std::unique_lock ul (m_ThreadMtx);
            for (auto & [sId, thread] : m_Threads) {
                close(sId);
                thread.join();
            }
            return;
        }

        CSocket socket = CSocket::Create(socketId);

        std::unique_lock ul (m_ThreadMtx);
        auto it = m_Threads.find(socketId);
        if (it != m_Threads.end()) {
            it->second.join();
            m_Threads.erase(it);
        }

        m_Threads.emplace(socketId, std::thread(serverThread, socket));
    }
}

void CSocketServer::Start(const std::function<void(CSocket)> & serverThread) {
    m_Receiver = std::thread(&CSocketServer::Receiver, this, std::cref(serverThread));
}

void CSocketServer::Stop() {
    std::unique_lock ul (m_ThreadMtx);

    for (auto & [socketId, thread] : m_Threads) {
        CSocket::Create(socketId).SendMessage(message_type_e::MSG_TYPE_MESSAGE, {"end", "end", "end"});
        close(socketId); // also stops thread
        thread.join();
    }

    m_Threads.clear();
    ul.unlock();

    close(m_Id);
    m_Receiver.join();
}
