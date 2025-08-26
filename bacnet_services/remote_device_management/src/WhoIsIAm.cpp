#include "WhoIsIAm.h"
#include "Networking.h"
#include <iostream>
#include <array>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
constexpr uint16_t BACNET_PORT = 47808;
constexpr std::array<uint8_t, 12> WHOIS_MESSAGE = {
    0x81, 0x0b, 0x00, 0x0c,
    0x01, 0x20,
    0xff, 0xff,
    0x00, 0x00,
    0x10, 0x08
};

WhoIsIAm::WhoIsIAm(Networking& net) : networking(net) {}

void WhoIsIAm::sendWhoIs() {
    if (!networking.sendTo(WHOIS_MESSAGE.data(), WHOIS_MESSAGE.size(), "255.255.255.255", BACNET_PORT)) {
        std::cerr << "Failed to send WhoIs message" << std::endl;
        return;
    }
    std::cout << "WhoIs message sent. Listening for responses..." << std::endl;
}

bool WhoIsIAm::receiveResponses() {
    responses.clear();
    int sock = networking.getSocket();
    if (sock < 0) return false;
#ifdef _WIN32
    DWORD timeout = 3000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));
#else
    timeval timeout{3, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#endif
    std::array<uint8_t, 512> buffer{};
    sockaddr_in from{};
    bool gotAny = false;
    while (true) {
#ifdef _WIN32
        int fromlen = sizeof(from);
#else
        socklen_t fromlen = sizeof(from);
#endif
        int received = recvfrom(sock, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0,
                                reinterpret_cast<sockaddr*>(&from), &fromlen);
        if (received > 0) {
            gotAny = true;
            Response resp;
#ifdef _WIN32
            char addrBuf[INET_ADDRSTRLEN] = {0};
            InetNtop(AF_INET, &from.sin_addr, addrBuf, INET_ADDRSTRLEN);
            resp.address = addrBuf;
#else
            char addrBuf[INET_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET, &from.sin_addr, addrBuf, INET_ADDRSTRLEN);
            resp.address = addrBuf;
#endif
            resp.data.assign(buffer.begin(), buffer.begin() + received);
            responses.push_back(std::move(resp));
        } else {
            break;
        }
    }
    return gotAny;
}

void WhoIsIAm::printResponses() const {
    if (responses.empty()) {
        std::cout << "No response received within timeout." << std::endl;
        return;
    }
    for (const auto& resp : responses) {
        std::cout << "Received response from " << resp.address << ":\n";
        const auto& data = resp.data;
        size_t len = data.size();
        if (len < 12) {
            std::cout << "Response too short to parse BACnet fields." << std::endl;
            for (auto byte : data) printf("%02X ", byte);
            std::cout << std::endl;
            continue;
        }

        if (isVerbose) {
            // BVLC
            std::cout << "  BVLC: Type: 0x" << std::hex << (int)data[0] << ", Function: 0x" << (int)data[1]
                      << ", Length: " << std::dec << ((data[2] << 8) | data[3]) << std::endl;
            // NPDU
            std::cout << "  NPDU: Version: 0x" << std::hex << (int)data[4] << ", Control: 0x" << (int)data[5] << std::endl;
            // APDU (I-Am expected)
            std::cout << "  APDU: PDU Type: 0x" << std::hex << (int)data[8] << ", Service Choice: 0x" << (int)data[9] << std::endl;
            // Print rest as raw hex with index
            std::cout << "  Raw Data:";
            for (size_t i = 0; i < data.size(); ++i) {
                if (i % 16 == 0) std::cout << "\n    ";
                printf("[%02zu]=%02X ", i, data[i]);
            }
            std::cout << std::endl;
            std::cout << "  (BVLC: BACnet Virtual Link Control, NPDU: Network Protocol Data Unit, APDU: Application Protocol Data Unit)" << std::endl;
        } 
        else {
            for (auto byte : data)
                printf("%02X ", byte);
            std::cout << std::endl;
        }
    }
}
