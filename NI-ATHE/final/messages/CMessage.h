#pragma once

#include <map>
#include <mutex>
#include <string>
#include <thread>

#include "../CSocket.h"
#include "../CUser.h"

class CMessage {
    size_t m_RequiredMessageCount;
public:
    CMessage (size_t requiredMessageCount)
    : m_RequiredMessageCount(requiredMessageCount) {}
    virtual ~CMessage() = default;
    size_t MessageCount() const {
        return m_RequiredMessageCount;
    }
    virtual bool ProcessMessage(std::map<CSocket, CUser> &, std::mutex &, CSocket &, const std::vector<std::string> &) const = 0;
};
