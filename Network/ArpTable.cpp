#include "Network/ArpTable.h"

namespace Network {

    void ArpTable::insertArpEntry(uint64_t &mac, Network::IpAddressV4 &address)
    {
        m_arpTable[address.data.word] = mac;
    }

    System::Error ArpTable::findArpEntry(uint64_t &mac, Network::IpAddressV4 &address)
    {
        ArpTableMap::iterator it;
        it = m_arpTable.find(address.data.word);
        if(it != m_arpTable.end())
        {
            mac = it->second;
            return System::Error::NO_ERROR;
        }

        return System::Error::ERR_ARP_NOT_FOUND;
    }

    System::Error ArpTable::removeArpEntry(Network::IpAddressV4 &address)
    {
        ArpTableMap::iterator it;
        it = m_arpTable.find(address.data.word);
        if(it != m_arpTable.end())
        {
            m_arpTable.erase(it);
            return System::Error::NO_ERROR;
        }

        return System::Error::ERR_ARP_NOT_FOUND;
    }
}