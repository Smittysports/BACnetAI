#pragma once
#include <cstdint>
#include <array>
#include <string>

/**
 * @class Networking
 * @brief Provides UDP networking functionality for BACnet communication.
 *
 * This class encapsulates socket creation, initialization, cleanup, and UDP datagram sending
 * for use by higher-level BACnet protocol classes. It is designed for cross-platform use (Windows and POSIX).
 *
 * Usage:
 *   - Construct a Networking object to initialize the UDP socket.
 *   - Use sendTo() to send datagrams to a specified address and port.
 *   - Use getSocket() to access the underlying socket for receiving data.
 */
class Networking {
public:
    /**
     * @brief Constructs and initializes the UDP socket.
     */
    Networking();

    /**
     * @brief Cleans up and closes the UDP socket.
     */
    ~Networking();

    /**
     * @brief Returns the underlying socket file descriptor/handle.
     * @return Socket descriptor (int), or -1 if not initialized.
     */
    int getSocket() const;

    /**
     * @brief Sends a UDP datagram to the specified IPv4 address and port.
     * @param data Pointer to the data buffer to send.
     * @param length Number of bytes to send.
     * @param ip Destination IPv4 address as a string (e.g., "192.168.1.255").
     * @param port Destination UDP port.
     * @return true if the data was sent successfully, false otherwise.
     */
    bool sendTo(const uint8_t* data, size_t length, const std::string& ip, uint16_t port);

private:
    int sock = -1;           ///< UDP socket file descriptor/handle.
    bool initialized = false;///< Indicates if the socket was successfully initialized.

    /**
     * @brief Initializes the UDP socket and prepares it for communication.
     */
    void init();

    /**
     * @brief Cleans up and closes the UDP socket.
     */
    void cleanup();
};
