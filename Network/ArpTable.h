//
// Created by marnold on 3/19/16.
//

#ifndef ROX_ARP_TABLE_H
#define ROX_ARP_TABLE_H
#include <stdint.h>
#include <map>
#include "Network/Ethernet.h"
#include "System/Error.h"
#include "../Network/IpAddressV4.h"


namespace Network
{
    typedef std::pair<uint32_t, uint64_t> ArpTableEntry;
    typedef std::map<uint32_t, uint64_t> ArpTableMap;

    class ArpTable
    {
    public:
        void insertArpEntry(uint64_t &mac, Network::IpAddressV4 &address);
        System::Error findArpEntry(uint64_t &mac, Network::IpAddressV4 &address);
        System::Error removeArpEntry(Network::IpAddressV4 &address);
        ArpTable(){}
        ~ArpTable(){}
    private:
        ArpTable(ArpTable& entry){}

        ArpTableMap m_arpTable;
    };
}

#endif //ROX_HEADERDECODER_H
