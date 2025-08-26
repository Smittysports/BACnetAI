
#include <iostream>
#include "../include/Networking.h"
#include "../bacnet_services/remote_device_management/include/WhoIsIAm.h"

int main() {
    std::cout << "Welcome to BACnetAI!" << std::endl;

    Networking net;
    WhoIsIAm whois(net);
    whois.sendWhoIs();
    if (whois.receiveResponses()) {
        whois.printResponses();
    } else {
        std::cout << "No BACnet devices responded to WhoIs." << std::endl;
    }
    return 0;
}

