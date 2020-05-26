#pragma once
#include "utils.h"

class Packet
{
public:
    struct IP_ADDRESS *destinationIP;
    struct IP_ADDRESS *sourceIP;
    struct MAC_ADDRESS *destination_mac_address;
    struct MAC_ADDRESS *source_mac_address;
    std::string content;

    Packet(std::string source, std::string destination, std::string cont);
    struct IP_ADDRESS extractSourceIP();

};