#pragma once

#include <map>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

#include "messages/CMessage.h"
#include "messages/CMessageChannel.h"
#include "messages/CMessageEnd.h"
#include "messages/CMessageUsername.h"
#include "messages/CMessageChannelChange.h"

#include "CSocket.h"
#include "CUser.h"

class CServer {
    CSocket m_Socket;
    static std::map<CSocket, CUser> m_Clients;
    static std::mutex m_ClientMtx;
    static std::map<uint32_t, std::shared_ptr<CMessage>> m_Processors;

    bool ProcessMessage(const message_header_t &);
    void Print(const std::string &) const;
public:
    explicit CServer (const CSocket & socket)
    : m_Socket(socket) {}

    void RunThread();
};
