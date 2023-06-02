#include "CServer.h"

std::map<CSocket, CUser> CServer::m_Clients;
std::mutex CServer::m_ClientMtx;

std::map<uint32_t, std::shared_ptr<CMessage>> CServer::m_Processors = {
    {message_type_e::MSG_TYPE_NOP, std::make_shared<CMessageEnd>()},
    {message_type_e::MSG_TYPE_CHANGE_NICK, std::make_shared<CMessageUsername>()},
    {message_type_e::MSG_TYPE_JOIN, std::make_shared<CMessageChannelChange>()},
    {message_type_e::MSG_TYPE_MESSAGE, std::make_shared<CMessageChannel>()}
};

void CServer::Print(const std::string & str) const {
    printf("%d: %s\n", m_Socket.Id(), str.c_str());
}

bool CServer::ProcessMessage(const message_header_t & messageHeader) {
    auto it = m_Processors.find(messageHeader.type);
    if (it == m_Processors.end()) // unknown message
        return Print("Unknown message type " + std::to_string(messageHeader.type)), true;

    const auto & processor = *(it->second);

    std::vector<std::string> messages;
    if ((processor.MessageCount() && !m_Socket.ReadMessage(messages, messageHeader.length - 8)) ||
        messages.size() != processor.MessageCount())
        return false;

    return processor.ProcessMessage(m_Clients, m_ClientMtx, m_Socket, messages);
}

void CServer::RunThread() {
    {
        std::unique_lock<std::mutex> ul (m_ClientMtx);
        m_Clients.emplace(m_Socket, CUser(m_Socket.Id(), "#public", ""));
    }

    message_header_t messageHeader {};
    while (m_Socket.ReadMessageHeader(messageHeader)) {
        if (Constants::DEBUG)
            Print("Received " + std::to_string(messageHeader.type));
        if (!ProcessMessage(messageHeader))
            break;
    }

    {
        std::unique_lock<std::mutex> ul (m_ClientMtx);
        m_Clients.erase(m_Socket);
    }

    Print("Connection closed.");
    close(m_Socket.Id());
}
