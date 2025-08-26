// Networking.cpp
#include "../include/Networking.h"
#include <iostream>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

constexpr uint16_t BACNET_PORT = 47808; // 0xBAC0

Networking::Networking() { init(); }
Networking::~Networking() { cleanup(); }

void Networking::init() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        sock = -1;
        initialized = false;
        return;
    }
#endif
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket!" << std::endl;
        initialized = false;
        return;
    }
    int broadcastEnable = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&broadcastEnable), sizeof(broadcastEnable));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(BACNET_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cerr << "Failed to bind socket!" << std::endl;
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        sock = -1;
        initialized = false;
        return;
    }
    initialized = true;
}

void Networking::cleanup() {
    if (sock >= 0) {
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        sock = -1;
        initialized = false;
    }
}

int Networking::getSocket() const {
    return sock;
}

bool Networking::sendTo(const uint8_t* data, size_t length, const std::string& ip, uint16_t port) {
    if (sock < 0 || !initialized) {
        std::cerr << "Socket not initialized!" << std::endl;
        return false;
    }
    sockaddr_in dest{};
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
#ifdef _WIN32
    if (ip == "255.255.255.255") {
        dest.sin_addr.s_addr = INADDR_BROADCAST;
    } else {
        InetPtonA(AF_INET, ip.c_str(), &dest.sin_addr);
    }
#else
    if (ip == "255.255.255.255") {
        dest.sin_addr.s_addr = INADDR_BROADCAST;
    } else {
        inet_pton(AF_INET, ip.c_str(), &dest.sin_addr);
    }
#endif
    int sent = sendto(sock, reinterpret_cast<const char*>(data), static_cast<int>(length), 0,
                      reinterpret_cast<sockaddr*>(&dest), sizeof(dest));
    if (sent < 0) {
        std::cerr << "Failed to send data to " << ip << ":" << port << std::endl;
        return false;
    }
    return true;
}
