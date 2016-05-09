#include "Network/ArpTable.h"

namespace Network {

    void ArpTable::insertArpEntry(uint64_t mac, uint32_t address)
    {

    }

    System::Error ArpTable::findArpEntry(uint64_t mac, uint32_t &address)
    {
        return System::Error::NO_ERROR;
    }

    void ArpTable::removeArpEntry(uint64_t mac)
    {

    }
}