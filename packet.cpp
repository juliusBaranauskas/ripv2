#include "packet.h"

Packet::Packet(std::string source, std::string destination, std::string cont)
{
    sourceIP = stringToIP(source);
    destinationIP = stringToIP(destination);
    content = cont;
}

struct IP_ADDRESS Packet::extractSourceIP()
{
    return *sourceIP;
}   