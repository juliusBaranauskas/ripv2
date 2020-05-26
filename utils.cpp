#include "utils.h"
#include "string"
#include "vector"
#include "sstream"

#define OCTET_COUNT 4

struct IP_ADDRESS* stringToIP(std::string stringIP)
{
    std::vector<std::string> octets = splitToOctets(stringIP);
    IP_ADDRESS *res = (IP_ADDRESS*)malloc(sizeof(IP_ADDRESS*));

    *res = IP_ADDRESS(toUINT8(octets[0]),
                    toUINT8(octets[1]),
                    toUINT8(octets[2]),
                    toUINT8(octets[3]));
    return res;
}

// TODO: add failsafes for too large, too little/many digits
std::vector<std::string> splitToOctets(std::string ip)
{
    std::vector<std::string> result;
    short int index;
    for(short int i = 0; i < OCTET_COUNT; ++i)
    {
        if ((index = ip.find('.')) != ip.length())
            result.push_back(ip.substr(0, index));
        else
            return result;

        ip = ip.substr(index+1);
    }

    return result;
}

// returns -1 on error
__uint8_t toUINT8(std::string octet)
{
    std::istringstream str(octet);
    int value;
    if( !(str >> value))
        return -1;

    return (value > 255 || value < 0) ? -1 : (__uint8_t)value;
}

struct IP_ADDRESS* getNextIP()
{
    lastIP->fourthOctet++;
    return lastIP;
}

void InitLog()
{
    LOG.open("log.txt", std::ios_base::app);
}

void log(std::string text)
{
    LOG << text;
}

bool emptyLog()
{
    return remove("log.txt") == 0;
}

void flushLog()
{
    LOG.flush();
}

bool areEqual(struct IP_ADDRESS* const a, struct IP_ADDRESS b)
{
    std::cout << "are equal?\n";
    return b.firstOctet == a->firstOctet
            && b.secondOctet == a->secondOctet
            && b.thirdOctet == a->thirdOctet
            && b.fourthOctet == a->fourthOctet;
}

bool areEqual(struct IP_ADDRESS* a, struct IP_ADDRESS* b)
{
    std::cout << "are equal? 2\n";
    return b->firstOctet == a->firstOctet
            && b->secondOctet == a->secondOctet
            && b->thirdOctet == a->thirdOctet
            && b->fourthOctet == a->fourthOctet;
}

std::pair<IP_ADDRESS *const, ROUTING_ENTRY> copyEntry(std::pair<IP_ADDRESS *const, ROUTING_ENTRY> pair)
{
    std::pair<IP_ADDRESS *const, ROUTING_ENTRY> newPair(pair.first, pair.second);
    return newPair;
}
