//
// Created by marnold on 3/19/16.
//

#ifndef ROX_ROUTE_TABLE_H
#define ROX_ROUTE_TABLE_H
#include <stdint.h>
#include <map>
#include "System/Error.h"
#include "System/LookupTree.h"

#include <iostream>

namespace Network
{
    uint32_t getHostMask(uint32_t address, uint32_t size);

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
        };

        RouteTable(RouteTable& entry){}

        System::LookupTree storage;
    };
}

#endif //ROX_HEADERDECODER_H
