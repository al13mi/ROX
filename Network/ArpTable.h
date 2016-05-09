//
// Created by marnold on 3/19/16.
//

#ifndef ROX_ARP_TABLE_H
#define ROX_ARP_TABLE_H
#include <stdint.h>
#include <map>
#include "Network/Ethernet.h"
#include "System/Error.h"

namespace Network
{
    class ArpTable
    {
    public:
        void insertArpEntry(uint64_t mac, uint32_t address);
        System::Error findArpEntry(uint64_t mac, uint32_t &address);
        void removeArpEntry(uint64_t mac);
        ArpTable(){}
        ~ArpTable(){}
    private:
        ArpTable(ArpTable& entry){}

        std::map<uint32_t, uint64_t> m_arpTable;
    };
}

#endif //ROX_HEADERDECODER_H
