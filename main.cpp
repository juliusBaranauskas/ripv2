#include "iostream"
#include "vector"
#include "fstream"
#include <unistd.h>
#include "string"
#include <stdlib.h>
#include <time.h>

#include "router.h"
#include "utils.h"
#include "packet.h"

#define time_interval 1000

void addDevices(int nRouters, std::vector<Router*> &devices);

void addNeighbours(int nRouters, std::vector<Router*> &routers, std::ifstream &file);

void printDevices(std::vector<Router*> routers);

void updateRoutingInfo(std::vector<Router*> &routerList);

int main() {
    emptyLog();
    InitLog();

    // used for counting routing table multicasts (all routers to all neighbours)
    short rtExchanges = 0;
    short nRouters;
    short timer = 0;
    std::string input;
    std::vector<Router*> routerList;
    std::ifstream file("graph.in");

    // reading from graph.in and creating objects to represent that data
    file >> nRouters;
    addDevices(nRouters, routerList);
    addNeighbours(nRouters, routerList, file);

    for(auto it: routerList)
    {
        log(it->m_name + "\n");
        for(auto tableEntry: it->routingTable)
        {
            log("\t" + tableEntry.first->toString() + " "
                + std::to_string((int)tableEntry.second.nextDeviceId) + " "
                + std::to_string((int)tableEntry.second.hop_count) + "\n");
        }
    }
    flushLog();
    srand(time(NULL));
    
    /*
    Main loop for:
        - updating router state
        - routing tables
        - requesting packets to be sent (console input)
    */
    while(true)
    {
        if (timer == 30)
        {
            // die or not to die
            for (auto it: routerList)
            {
                if (rand()%1000 == 1)
                {
                    it->isWorking = false;
                    log(it->m_name + " died\n");
                }
            }

            // all the routers multicast their routing tables to their neighbours
            updateRoutingInfo(routerList);
            timer = 0;
            rtExchanges++;
        }

        if (rtExchanges == 3)
        {
            std::cout << "Enter cmd \'ping source_ip destination_ip packet_message\'\n";
            std::getline(std::cin, input);
            if (input.substr(0, 4) == "ping")
            {
                std::cout << "ping cmd init...\n";

                short firstWhitespace = input.find(' ');
                short secondWhitespace = input.find(' ', firstWhitespace+1);
                short thirdWhitespace = input.find(' ', secondWhitespace+1);
                std::string firstIp = input.substr(firstWhitespace+1, secondWhitespace-firstWhitespace-1);
                std::string secondIp = input.substr(secondWhitespace+1, thirdWhitespace-secondWhitespace-1);
                std::string message = input.substr(thirdWhitespace+1);
                std::cout << "source: " << firstIp << std::endl;
                std::cout << "dest: " << secondIp << std::endl;

                // create a packet
                Packet packet(firstIp, secondIp, message);
                short index = -1;
                auto srcIp =  stringToIP(firstIp);

                // find first recipient and begin sending
                for(auto it: routerList)
                {
                    if (areEqual(it->directConnection, srcIp))
                    {
                        index = it->m_id;
                        break;
                    }
                }

                bool sending;
                if (index != -1)
                {
                    sending = true;
                    log("\n");
                    while (sending)
                    {
                        index = routerList[index]->receivePacket(packet);
                        if (index == -1 || index == -2)
                            sending = false;
                    }
                    flushLog();
                }
            }
        }
        
        usleep(time_interval);
        timer++;
    }

    return 0;
}

void addDevices(int nRouters, std::vector<Router*> &routers)
{
    int i;
    for(i = 0; i < nRouters; ++i)
    {
        Router *router = new Router();
        router->generateNextName();
        routers.push_back(router);
    }
    log("finished adding devices\n");
}

void addNeighbours(int nRouters, std::vector<Router*> &routers, std::ifstream &file)
{
    int nConn;
    int routerIndex;
    std::string ip_addr;
    int j;

    for(int i = 0; i < nRouters; ++i)
    {
        file >> nConn;
        for(j = 0; j < nConn; ++j)
        {
            file >> routerIndex;
            routers[i]->m_neighbours.push_back(routers[routerIndex]);
        }

        file >> nConn;
        for(j = 0; j < nConn; ++j)
        {
            file >> ip_addr;
            log("adding direct conn: " + ip_addr + " to " + routers[i]->m_name + "\n");

            struct ROUTING_ENTRY *entry = new struct ROUTING_ENTRY();
            entry->nextDeviceId = routers[i]->m_id;
            entry->currentDeviceId = routers[i]->m_id;
            entry->hop_count = 0;
            entry->ip_address = stringToIP(ip_addr);
            routers[i]->routingTable.insert(std::pair<struct IP_ADDRESS*, struct ROUTING_ENTRY>(stringToIP(ip_addr), *entry));

            routers[i]->directConnection = entry->ip_address;
            log("added to routing table " + std::to_string(routers[i]->routingTable.size()) + "\n");
        }
    }
    log("finished adding neighbours\n");
}

void printDevices(std::vector<Router*> routers)
{
    for(auto it: routers)
        std::cout << it->m_name << std::endl;

    std::cout << std::endl;
}

void updateRoutingInfo(std::vector<Router*> &routerList)
{
    flushLog();
    for(auto it: routerList)
    {
        if(!it->isWorking)
            continue;

        for(auto neighbour: it->m_neighbours)
            if (neighbour->isWorking)
                neighbour->acceptRoutingTable(it->routingTable);

        log(it->m_name + "\n");

        for(auto tableEntry: it->routingTable)
            log("\t" + tableEntry.first->toString() + " "
                + std::to_string((int)tableEntry.second.nextDeviceId) + " "
                + std::to_string((int)tableEntry.second.hop_count) + "\n");
    }
}