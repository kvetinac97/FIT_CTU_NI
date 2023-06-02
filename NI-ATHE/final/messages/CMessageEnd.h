#pragma once

#include "CMessage.h"

class CMessageEnd: public CMessage {
public:
    CMessageEnd() : CMessage(0) {}
    bool ProcessMessage(std::map<CSocket, CUser> &, std::mutex &, CSocket &, const std::vector<std::string> &) const override {
        return false; // stop
    }
};
