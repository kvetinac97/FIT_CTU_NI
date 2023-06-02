#include "CSocket.h"

CSocket::CSocket() {
    m_Id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_Id < 0)
        throw SocketCreateException();
}

CSocket CSocket::Create(int id) {
    return CSocket(id);
}

int CSocket::Id() const {
    return m_Id;
}

bool CSocket::ReadMessageHeader(message_header_t &messageHeader) const {
    ssize_t x = recv(m_Id, &messageHeader, sizeof(messageHeader), 0);
    if (Constants::DEBUG)
        printf("Return value: %zd %u %u\n", x, messageHeader.length, messageHeader.type);
    return x == sizeof(messageHeader);
}

bool CSocket::ReadMessage(std::vector<std::string> & messages, uint32_t messageLen) const {
    if (Constants::DEBUG)
        printf("Reading %u characters...\n", messageLen);
    if (messageLen == 0) return true;
    char * originalBuffer = new char[messageLen + 1];
    if (recv(m_Id, originalBuffer, messageLen, MSG_WAITALL) <= 0) {
        delete [] originalBuffer;
        return false;
    }
    originalBuffer[messageLen] = '\0';
    char * buffer = originalBuffer;

    messages.clear();
    while (buffer < originalBuffer + messageLen) {
        size_t len = strlen(buffer);
        if (!len) break;

        if (Constants::DEBUG)
            printf("Read %s from %d\n", buffer, m_Id);

        messages.emplace_back(buffer);
        buffer += len + 1;
    }

    delete [] originalBuffer;
    return true;
}

bool CSocket::SendMessage(uint32_t messageId, const std::vector<std::string> & messages) const {
    uint32_t totalMessageLen = 0;
    for (const auto & msg : messages)
        totalMessageLen += msg.size() + 1;

    message_header_t header { 4 + 4 + totalMessageLen, messageId };
    if (Constants::DEBUG)
        printf("Sending header (length %d) %d to %d\n", header.length, header.type, m_Id);
    ssize_t sentMID = send(m_Id, &header, sizeof(header), MSG_NOSIGNAL);
    if (sentMID != sizeof(header))
        return false;

    return std::all_of(messages.begin(), messages.end(), [&](const auto & msg) {
        if (Constants::DEBUG)
            printf("Sending %s to %d\n", msg.c_str(), m_Id);
        ssize_t sentMsg = send(m_Id, msg.c_str(), msg.size() + 1, MSG_NOSIGNAL); // send also '\0'
        return sentMsg == (ssize_t) msg.size() + 1;
    });
}

bool CSocket::operator < (const CSocket & other) const {
    return m_Id < other.m_Id;
}
