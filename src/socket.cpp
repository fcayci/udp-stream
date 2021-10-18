#include "socket.h"

#include <iostream>

Socket::Socket(int type, int protocol){
    sockfd = socket(AF_INET, type, protocol);
}

Socket::~Socket() {
  ::close(sockfd);
  sockfd = -1;
}

void Socket::setLocalPort(uint32_t localPort) {
    sockaddr_in localAddress;
    memset(&localAddress, 0, sizeof localAddress);

    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(localPort);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    int status = bind(sockfd, (sockaddr *) &localAddress, sizeof(sockaddr_in));
    if (status < 0) {
        std::cout << "Bind failed\n";
    }
}

UDPSocket::UDPSocket() : Socket(SOCK_DGRAM, IPPROTO_UDP) {
    memset(&to, 0, sizeof to);
}

UDPSocket::~UDPSocket() {
}

void UDPSocket::setDestination(const std::string& destAddr, size_t destPort) {
    to.sin_family = AF_INET;
    to.sin_port = htons(destPort);
    to.sin_addr.s_addr = inet_addr(destAddr.c_str());
}


void UDPSocket::send(const void* buf, size_t bufLen) {
    if (to.sin_port == 0) {
        std::cout << "first set destination address with setDestionation\n";
        std::cout << "port : " << to.sin_port << "\n";
        return;
    }
    if (bufLen > PACKET_SIZE) {
        std::cout << "Too big to send in one chunk, truncating\n";
    }
    int totalPacket =  1 + (bufLen - 1) / PACKET_SIZE;

    for (int i = 0; i < totalPacket; ++i) {
        ::sendto(sockfd, &((char *)buf)[i * PACKET_SIZE], PACKET_SIZE, 0, (struct sockaddr *) &to, sizeof(struct sockaddr));
    }
}


void UDPSocket::sendFrame(const void* buf, size_t bufLen) {
    if (to.sin_port == 0) {
        std::cout << "first set destination port with setDestionation\n";
        std::cout << "port : " << to.sin_port << "\n";
        return;
    }
    int totalPacket =  1 + (bufLen - 1) / PACKET_SIZE;

    /* let it be known this many packets will come */
    ::sendto(sockfd, &totalPacket, 4, 0, (struct sockaddr *) &to, sizeof(struct sockaddr));

    for (int i = 0; i < totalPacket; ++i) {
        ::sendto(sockfd, &((char *)buf)[i * PACKET_SIZE], PACKET_SIZE, 0, (struct sockaddr *) &to, sizeof(struct sockaddr));
    }
}


int UDPSocket::receive(void *buf) {
    struct sockaddr_in from;
    memset(&from, 0, sizeof from);
    socklen_t fromLen = sizeof(struct sockaddr);
    int recvSize = ::recvfrom(sockfd, (char *)buf, PACKET_SIZE, 0, (sockaddr *) &from, (socklen_t *) &fromLen);
    return recvSize;
}


int UDPSocket::receiveFrame(void *buf) {
    struct sockaddr_in from;
    memset(&from, 0, sizeof from);
    socklen_t fromLen = sizeof(struct sockaddr);
    int totalPacket = 0;
    ::recvfrom(sockfd, &totalPacket, 4, 0, (sockaddr *) &from, (socklen_t *) &fromLen);
    int recvSize = 0;
    for (int i=0; i < totalPacket; ++i) {
        recvSize += ::recvfrom(sockfd, &((char *)buf)[i * PACKET_SIZE], PACKET_SIZE, 0, (sockaddr *) &from, (socklen_t *) &fromLen);
    }
    return recvSize;
}
