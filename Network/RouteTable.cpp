#include "Network/RouteTable.h"
namespace Network
{
    uint64_t getKey(uint32_t address, uint32_t size)
    {
        uint32_t mask = 0xFFFFFFFF >> (32 - size);
        mask = mask << (32 - size);
        uint64_t key = (uint64_t)(mask & address) << 32;
        key = key | size;
        return key;
    }

    void RouteTable::insertRoute(uint32_t address, uint32_t size, uint32_t nextHop)
    {
        uint64_t key = getKey(address, size);
        m_routeTable.insert(std::make_pair(key, nextHop));
    }

    void RouteTable::removeRoute(uint32_t address, uint32_t size)
    {
        std::map<uint32_t, uint32_t>::iterator it = m_routeTable.find(getKey(address, size));
        m_routeTable.erase(it);
    }

    System::Error RouteTable::findNextHop(uint32_t address, uint32_t &nextHop)
    {
        std::map<uint32_t, uint32_t>::iterator it = m_routeTable.find(getKey(address, 32));
        if(it != m_routeTable.end())
        {
            nextHop = it->second;
            return System::Error::NO_ERROR;
        }

        return System::Error::ERR_ROUTE_NOT_FOUND;
    }

}