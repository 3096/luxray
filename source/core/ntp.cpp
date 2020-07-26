/* This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>

Additionally, the ntp_packet struct uses code licensed under the BSD 3-clause. See LICENSE-THIRD-PARTY for more
information. */

#define _BSD_SOURCE

#include "ntp.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <switch.h>
#include <sys/socket.h>

#include <cstring>
#include <stdexcept>
#include <string>

#include "lx/debug.hpp"

time_t ntpGetTime() {
    static const char* SERVER_NAME = "0.pool.ntp.org";
    static const uint16_t PORT = 123;

    int sockfd = -1;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        std::string msg = "Failed to open socket with error code " + std::to_string(errno);
        throw std::runtime_error(msg);
    }
    LOG("Opened socket\nAttempting to connect to %s", SERVER_NAME);

    struct hostent* server;
    errno = 0;
    if ((server = gethostbyname(SERVER_NAME)) == NULL) {
        std::string msg = "Gethostbyname failed: " + std::to_string(errno);
        throw std::runtime_error(msg);
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    memcpy((char*)&serv_addr.sin_addr.s_addr, (char*)server->h_addr_list[0], 4);
    serv_addr.sin_port = htons(PORT);

    errno = 0;
    int res = 0;
    if ((res = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        std::string msg = "Connect failed: " + std::to_string(res);
        msg += " errno: " + std::to_string(errno);
        throw std::runtime_error(msg);
    }
    LOG("Connected to 0.pool.ntp.org with result: %x %x\nSending time request...", res, errno);

    ntp_packet packet;
    memset(&packet, 0, sizeof(ntp_packet));
    packet.li_vn_mode = (0 << 6) | (4 << 3) | 3;              // LI 0 | Client version 4 | Mode 3
    packet.txTm_s = htonl(NTP_TIMESTAMP_DELTA + time(NULL));  // Current networktime on the console
    errno = 0;
    if ((res = send(sockfd, (char*)&packet, sizeof(ntp_packet), 0)) < 0) {
        std::string msg = "Error writing to socket: " + std::to_string(res);
        msg += " errno: " + std::to_string(errno);
        throw std::runtime_error(msg);
    }
    LOG("Sent time request with result: %x %x, waiting for response...", res, errno);

    errno = 0;
    if ((size_t)(res = recv(sockfd, (char*)&packet, sizeof(ntp_packet), 0)) < sizeof(ntp_packet)) {
        std::string msg = "Error reading from socket: " + std::to_string(res);
        msg += " errno: " + std::to_string(errno);
        throw std::runtime_error(msg);
    }
    packet.txTm_s = ntohl(packet.txTm_s);

    return (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);
}
