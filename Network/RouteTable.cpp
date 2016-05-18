#include "Network/RouteTable.h"
namespace Network
{
    uint32_t getHostMask(uint32_t address, uint32_t size)
    {
        uint32_t mask = 0xFFFFFFFF >> (32 - size);
        mask = mask << (32 - size);
        uint32_t key = mask & address;
        return key;
    }

    void RouteTable::insertRoute(uint32_t address, uint32_t size, uint32_t nextHop)
    {
    }

    void RouteTable::removeRoute(uint32_t address, uint32_t size)
    {

    }

    System::Error RouteTable::findNextHop(uint32_t address, uint32_t &nextHop)
    {
        return System::Error::ERR_ROUTE_NOT_FOUND;
    }

}