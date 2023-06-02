#include "CSocketClient.h"

CSocketClient::CSocketClient() : CSocket(), m_Stopped(false) {}

bool CSocketClient::Connect(const char * hostname, int port) {
    socklen_t socketAddressLen;
    socketAddressLen = sizeof(struct sockaddr_in);

    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    struct hostent *host;
    host = gethostbyname(hostname);
    memcpy(&serverAddr.sin_addr, host->h_addr,host->h_length);

    // Connection
    return connect(m_Id, (struct sockaddr *) &serverAddr, socketAddressLen) >= 0;
}

void CSocketClient::Reset() {
    m_Id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

bool CSocketClient::SetUsername (const std::string & username) {
    return SendMessage(message_type_e::MSG_TYPE_CHANGE_NICK, {username});
}

bool CSocketClient::JoinChannel(const std::string & channel) {
    return SendMessage(message_type_e::MSG_TYPE_JOIN, {channel});
}

void CSocketClient::Receiver() {
    while (true) {
        message_header_t messageHeader {};
        if (!ReadMessageHeader(messageHeader))
            return (void) printf("END: Could not read message header\n");

        std::vector<std::string> messages;
        bool result;
        switch (messageHeader.type) {
            case message_type_e::MSG_TYPE_MESSAGE:
                if (!ReadMessage(messages, messageHeader.length - 8) || messages.size() != 3)
                    return (void) printf("END: Could not read message message\n"); // error, end
                if (messages[1] == "end") { // stop sequence
                    m_Stopped = true;
                    return;
                }
                printf("%s > %s: %s\n", messages[1].c_str(), messages[0].c_str(), messages[2].c_str());
                break;
            case message_type_e::MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK:
                // taken username = "false"
                result = ReadMessage(messages, messageHeader.length - 8) &&
                        messages.size() == 2 && messages[1] == "true";
                if (result)
                    m_Username = messages[0];
                printf("Username change %s.\n", result ? "successful" : "failed");
                break;
            case message_type_e::MSG_TYPE_ACKNOWLEDGE_JOIN:
                // bad channel = "false"
                result = ReadMessage(messages, messageHeader.length - 8) &&
                        messages.size() == 2 && messages[1] == "true";
                if (result)
                    m_Channel = messages[0];
                printf("Channel change %s.\n", result ? "successful" : "failed");
                break;
        }
    }
}

void CSocketClient::Start() {
    m_Receiver = std::thread(&CSocketClient::Receiver, this);
    std::string message;
    while(true){
        if (!getline(std::cin, message) || message == Constants::COMMAND_STOP) {
            printf("Closing connection.\n");
            SendMessage(message_type_e::MSG_TYPE_NOP, {});
            m_Receiver.join();
            return;
        }

        if (m_Stopped) {
            printf("Connection closed by server.\n");
            Stop();
            return;
        }

        // regular message
        if (message.size() < Constants::COMMAND_LEN || (!message.empty() && (message[0] != '/'))) {
            if (!SendMessage(message_type_e::MSG_TYPE_MESSAGE, {m_Username, m_Channel, message}))
                printf("Error whilst sending message: try again.\n");
            continue;
        }

        // private message
        if (message.substr(0, Constants::COMMAND_LEN) == Constants::COMMAND_DM) {
            std::stringstream ss (message.substr(Constants::COMMAND_LEN));
            std::string username, rawMessage;

            if (!(ss >> username >> std::ws)) {
                printf("Could not parse username for private message.\n");
                continue;
            }

            std::getline(ss, rawMessage);
            if (!SendMessage(message_type_e::MSG_TYPE_MESSAGE, {m_Username, username, rawMessage}))
                printf("Error whilst sending private message: try again.\n");
            continue;
        }

        ProcessCommand(message);
    }
}

void CSocketClient::ProcessCommand(const std::string & message) {
    std::string command = message.substr(0, Constants::COMMAND_LEN),
            withoutCommand = message.substr(Constants::COMMAND_LEN);

    if (command == Constants::COMMAND_HELP) {
        printf("Usage:\n"
               "<message>             ... send message to channel you are in\n"
               "/usr <username>       ... change username\n"
               "/prv <usr> <msg>      ... send private message\n"
               "/chn <channel>        ... change channel\n"
               "/hlp                  ... display help\n"
               "/end                  ... end connection\n");
        return;
    }

    if (!withoutCommand.empty() && withoutCommand[0] == ' ')
        withoutCommand = withoutCommand.substr(1);

    if (command == Constants::COMMAND_CHG_CHL)
        printf("Channel join %s\n", JoinChannel(withoutCommand) ? "successful" : "failed");
    else if (command == Constants::COMMAND_USERNAME)
        printf("%s\n", SetUsername(withoutCommand) && JoinChannel(m_Channel) ?
            "Username change successful." : "Username change failed: already taken.");
    else
        printf("Unrecognized command.\n");
}

void CSocketClient::Stop() {
    m_Receiver.join();
    SendMessage(message_type_e::MSG_TYPE_NOP, {});
    close(m_Id);
}
