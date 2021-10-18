#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

class Socket {
public:
    Socket() = delete;
    Socket(int type, int protocol);
    void setLocalPort(uint32_t localPort);
    ~Socket();
protected:
    static const int PACKET_SIZE = 4096;
    int sockfd;
};


class UDPSocket : public Socket {
public:
    UDPSocket();
    UDPSocket(int localPort);
    ~UDPSocket();

    void setDestination(const std::string& destAddr, size_t destPort);
    void send(const void* buf, size_t bufLen);
    void sendFrame(const void* buf, size_t bufLen);
    int receive(void *buf);
    int receiveFrame(void *buf);
private:
    struct sockaddr_in to;
};

#endif // SOCKET_H_