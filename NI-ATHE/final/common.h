#pragma once

#include <stdexcept>
#include <sstream>
#include <string>

namespace Constants {
    static const char * DEFAULT_HOSTNAME = "localhost";
    static const int DEFAULT_SERVER_PORT = 443;
    static const bool DEBUG              = true;

    static const char * COMMAND_STOP      = "/end";
    static const char * COMMAND_USERNAME  = "/usr";
    static const char * COMMAND_CHG_CHL   = "/chn";
    static const char * COMMAND_DM        = "/prv";
    static const char * COMMAND_HELP      = "/hlp";
    static const size_t COMMAND_LEN = 4;
}

class SocketException: public std::runtime_error {
protected:
    explicit SocketException(const char * what)
    : std::runtime_error(what) {}
};

class SocketCreateException : public SocketException {
public:
    SocketCreateException()
    : SocketException("Could not Create socket.") {}
};

class SocketBindException : public SocketException {
public:
    explicit SocketBindException(int port)
    : SocketException(("Could not bind to port " + std::to_string(port) + ".").c_str()) {}
};

class SocketPassiveException : public SocketException {
public:
    SocketPassiveException()
    : SocketException("Could not mark socket as passive.") {}
};
