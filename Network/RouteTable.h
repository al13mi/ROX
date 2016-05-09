//
// Created by marnold on 3/19/16.
//

#ifndef ROX_ROUTE_TABLE_H
#define ROX_ROUTE_TABLE_H
#include <stdint.h>
#include <map>
#include "System/Error.h"

namespace Network
{
    uint64_t getKey(uint32_t address, uint32_t size);

    class RouteTable
    {
    public:
        void insertRoute(uint32_t address, uint32_t size, uint32_t nextHop);
        System::Error findNextHop(uint32_t address, uint32_t &nextHop);
        void removeRoute(uint32_t address, uint32_t size);
        RouteTable(){}
        ~RouteTable(){}
    private:
        struct RouteComparator
        {
            bool operator()(const uint64_t &left, const uint64_t & right) const
            {
                uint32_t left_network = left >> 32;
                uint32_t size = left_network & 0xFFFFFFFF;

                uint32_t right_network = getKey( right >> 32, size);
                return left_network > right_network;
            }
        };

        RouteTable(RouteTable& entry){}

        std::map<uint32_t, uint32_t, RouteComparator> m_routeTable;
    };
}

#endif //ROX_HEADERDECODER_H
