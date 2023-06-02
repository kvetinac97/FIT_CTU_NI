#ifndef PROTO_H_
#define PROTO_H_

// needed to use RTTI
#include <typeinfo>

#include <cassert>
#include <stdexcept>
#include <sys/fcntl.h>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>

using byte = unsigned char;

enum message_type_e : uint32_t {
	MSG_TYPE_NOP                        = 0, // not used
	MSG_TYPE_PUBLIC                     = 1, // not used
	MSG_TYPE_CHANGE_NICK                = 2, // change nickname (string)
	MSG_TYPE_PRIVATE                    = 3, // not used
	MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK    = 4, // acknowledge change nick (string,string)
	MSG_TYPE_MESSAGE                    = 5, // message (string,string,string)
	MSG_TYPE_JOIN                       = 6, // join channel (string)
	MSG_TYPE_PING                       = 7, // used to check if client is alive
	MSG_TYPE_PONG                       = 8, // not used
	MSG_TYPE_ACKNOWLEDGE_JOIN           = 9  // acknowledge join channel
};

struct message_header_t {
	uint32_t length;                // message length
	uint32_t type;                  // message_type_e
} __attribute__((packed)); /* do not use padding for alignment */

#endif /* PROTO_H_ */
