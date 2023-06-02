#pragma once

#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "common.h"
#include "tp.h"

class CSocket {
    explicit CSocket(int id) : m_Id(id) {}
protected:
    int m_Id;
    CSocket();
public:
    int Id() const;
    virtual ~CSocket() = default;
    bool operator < (const CSocket &) const;
    static CSocket Create(int id);
    virtual void Stop() {};

    bool ReadMessageHeader(message_header_t & messageHeader) const;
    bool ReadMessage(std::vector<std::string> & messages, uint32_t) const;

    bool SendMessage(uint32_t, const std::vector<std::string> &) const;
};
