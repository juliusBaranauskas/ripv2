#pragma once
#include <stdint.h>
#include "vector"
#include "string"
#include "fstream"
#include "sstream"
#include "iostream"

struct IP_ADDRESS
{
    IP_ADDRESS(__uint8_t first, __uint8_t second, __uint8_t third, __uint8_t fourth) :
        firstOctet(first),
        secondOctet(second),
        thirdOctet(third),
        fourthOctet(fourth)
        {
        }

    __uint8_t firstOctet;
    __uint8_t secondOctet;
    __uint8_t thirdOctet;
    __uint8_t fourthOctet;
public:
    std::string toString()
    {
        std::stringstream str[4];
        str[0] << (int)firstOctet;
        str[1] << (int)secondOctet;
        str[2] << (int)thirdOctet;
        str[3] << (int)fourthOctet;

        std::string temp;
        std::string res = "";
        for(int i = 0; i < 4; i++)
        {
            if (i != 0)
                res += '.';

            str[i] >> temp;
            res += temp;

        }
        return res;
    }

    bool operator==(const struct IP_ADDRESS a)
    {
        std::cout << "anuthig\n";
        return firstOctet == a.firstOctet
            && secondOctet == a.secondOctet
            && thirdOctet == a.thirdOctet
            && fourthOctet == a.fourthOctet;
    }
};

struct ROUTING_ENTRY{
    struct IP_ADDRESS* ip_address;
    short hop_count;
    short nextDeviceId;
    short currentDeviceId;
    bool empty = false;
};

struct MAC_ADDRESS
{
    MAC_ADDRESS(std::string addr)
    {
        if (addr.length() >= 12)
            for(int i = 0; i < 12; ++i)
                hex_numbers[i] = addr[i];
        else
            for(int i = 0; i < 12; ++i)
                hex_numbers[i] = '0';
    }

    char hex_numbers[12];

    bool operator==(const struct MAC_ADDRESS a)
    {
        for(int i = 0; i < 12; ++i)
            if (hex_numbers[i] != a.hex_numbers[i])
                return false;

        return true;
    }
};

std::vector<std::string> splitToOctets(std::string ip);
// Converts string representation of IP address to IP_ADDRESS struct object
// e.g. "192.168.0.12"
struct IP_ADDRESS* stringToIP(std::string stringIP);

__uint8_t toUINT8(std::string octet);

struct IP_ADDRESS* getNextIP();

static struct IP_ADDRESS* lastIP = stringToIP("192.168.0.1");

static std::ofstream LOG;

void log(std::string text);

void InitLog();

bool emptyLog();

void flushLog();

bool areEqual(struct IP_ADDRESS* const a, struct IP_ADDRESS b);

bool areEqual(struct IP_ADDRESS* a, struct IP_ADDRESS* b);

std::pair<IP_ADDRESS *const, ROUTING_ENTRY> copyEntry(std::pair<IP_ADDRESS *const, ROUTING_ENTRY> pair);