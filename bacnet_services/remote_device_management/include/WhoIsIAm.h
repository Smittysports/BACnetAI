
#pragma once
#include <vector>
#include <string>
#include <stdint.h>

class Networking;

/**
 * @class WhoIsIAm
 * @brief Handles sending BACnet WhoIs requests and receiving I-Am responses.
 *
 * This class provides methods to send a BACnet WhoIs broadcast message over the network,
 * receive any number of I-Am responses from BACnet devices, and print the responses
 * in either verbose or hex format.
 *
 * Usage:
 *   - Construct with a reference to a Networking object.
 *   - Call sendWhoIs() to broadcast a WhoIs message.
 *   - Call receiveResponses() to collect responses.
 *   - Call printResponses() to display the collected responses.
 */
class WhoIsIAm {
public:
    /**
     * @brief Constructs a WhoIsIAm object using the provided Networking instance.
     * @param net Reference to a Networking object for UDP communication.
     */
    WhoIsIAm(Networking& net);

    /**
     * @brief Sends a BACnet WhoIs broadcast message.
     */
    void sendWhoIs();

    /**
     * @brief Receives I-Am responses from BACnet devices.
     * @return true if at least one response was received, false otherwise.
     */
    bool receiveResponses();

    /**
     * @brief Prints all received responses.
     *        If isVerbose is true, prints detailed BACnet field breakdown.
     *        Otherwise, prints a hex dump of the response.
     */
    void printResponses() const;

    /**
     * @struct Response
     * @brief Represents a single BACnet I-Am response.
     */
    struct Response {
        std::string address;         ///< Source IP address of the response.
        std::vector<uint8_t> data;   ///< Raw response data.
    };

private:
    Networking& networking;              ///< Reference to the networking layer.
    std::vector<Response> responses;     ///< Collected I-Am responses.
    bool isVerbose {false};              ///< Verbose output flag.
};
