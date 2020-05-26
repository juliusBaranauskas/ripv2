#pragma once
#include "packet.h"
#include "utils.h"
#include "map"
#include "set"
static int m_lastDeviceId = 0;


class Router
{
public:
    std::string m_name;
    std::vector<Router*> m_neighbours;
    struct IP_ADDRESS* directConnection;
    short m_id;
    bool isWorking = true;
    std::vector<struct IP_ADDRESS*> interfaces; // ip addresses of router interfaces
    std::map<struct IP_ADDRESS*, struct ROUTING_ENTRY> routingTable; // info where to exit if we have destination ip_Addr 

    Router();
    int receivePacket(Packet packet);
    void generateNextName();
    void acceptRoutingTable(std::map<struct IP_ADDRESS*, struct ROUTING_ENTRY> routingTableOffer);
    struct ROUTING_ENTRY findInRoutingTable(struct IP_ADDRESS* entry);
};