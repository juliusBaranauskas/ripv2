#include "router.h"
#include "sstream"

Router::Router()
{

}

// returns next hop router id, -1 when it is not found, -2 when packet has reached it's destination
int Router::receivePacket(Packet packet)
{
    if (!isWorking)
    {
        log(m_name + " cannot be reached...\n");
        return -1;
    }

    log(m_name + " received packet with: " + packet.content + "\n");
    flushLog();

    ROUTING_ENTRY entry = findInRoutingTable(packet.destinationIP);
    if (!entry.empty)
    {
        std::cout << ("table has this ip: " + packet.destinationIP->toString() + " with hop " + std::to_string((int)entry.hop_count) + "\n");
        // is the device connected directly?
        if ((int)entry.hop_count == 0)
        {
            std::cout << ("packet found it's destination at: " + m_name);
            log("packet found it's destination at: " + m_name  + "\n");
            // add success icmp => asks to create confirm packet with field Confirm=true
            return -2;
        }
        else
            return entry.nextDeviceId;
    }

    log("error: sending terminated, router doesn't know where next to send\n");
    return -1;
}

void Router::generateNextName()
{
    m_id = m_lastDeviceId;
    std::stringstream ss;
    ss << m_lastDeviceId;
    m_name = "Router_" + ss.str();
    m_lastDeviceId++;
}

void Router::acceptRoutingTable(std::map<struct IP_ADDRESS*, struct ROUTING_ENTRY> routingTableOffer, bool works /* = true*/)
{
    log(m_name + " routing table size: " + std::to_string(routingTable.size()) + "\n");

    if (routingTableOffer.empty())
        return;

    if (!works)
    {
        for (auto it: routingTable)
            if (it.second.nextDeviceId == routingTableOffer[0].currentDeviceId)
                routingTable.erase(it.first);

        return;
    }

    ROUTING_ENTRY entry;

    for (auto it: routingTableOffer)
    {
        entry = findInRoutingTable(it.first);
        if (!entry.empty)
        {
            if (entry.hop_count > it.second.hop_count+1 && it.second.hop_count+1 < 16){
                routingTable.erase(it.first);
                
                auto newEntry = copyEntry(it);
                newEntry.second.hop_count++;
                newEntry.second.nextDeviceId = it.second.currentDeviceId;
                newEntry.second.currentDeviceId = m_id;

                routingTable.insert(newEntry);
                log("Replacing in RT since lower hop\n");
            }
        }
        else
        {
            if (it.second.hop_count > 14)
                return;
            auto newEntry = copyEntry(it);
            newEntry.second.hop_count++;
            newEntry.second.nextDeviceId = it.second.currentDeviceId;
            newEntry.second.currentDeviceId = m_id;
            routingTable.insert(newEntry);
            log("Entry not in RT\n");
        }
    }
}

struct ROUTING_ENTRY Router::findInRoutingTable(struct IP_ADDRESS* entry)
{
    std::cout << routingTable.size()<<std::endl;
    for(auto it: routingTable)
        if (areEqual(it.first, *entry))
            return it.second;

    ROUTING_ENTRY entr;
    entr.empty = true;
    return entr;
}