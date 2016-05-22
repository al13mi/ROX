#ifndef IF_INTERFACE_MANAGER
#define IF_INTERFACE_MANAGER

#include <map>
#include "Error.h"
#include "Network/IpAddressV4.h"
#include "FlowManager.h"

namespace System
{
    typedef std::pair<uint32_t, std::unique_ptr<IfInterface>> IfInterfaceMapEntry;
    typedef std::map<IfInterfaceMapEntry> RouteToIfInterfaceMap;
    class IfInterfaceManager
    {
    public:
        IfInterfaceManager()
        {
        }

        Error findIfInterface(const Network::IpAddressV4 &route, IfInterface &ifInterface);
        Error addIfInterface(const Network::IpAddressV4 &route, IfInterface &ifInterface);
        Error removeIfInterface(const Network::IpAddressV4 &route);
    private:
        RouteToIfInterfaceMap m_routeToIfInterfaceMap;
    };
}

#endif