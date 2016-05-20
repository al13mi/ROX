#ifndef TREE_H
#define TREE_H

#include <map>
#include "../Network/IpAddressV4.h"
#include "Error.h"
#include <memory>

namespace System
{
    struct Node
    {
        Node();
        Node(const Network::IpAddressV4 &address);
        ~Node();

        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Network::IpAddressV4 nextHop;
        bool end;
    };

    class LookupTree
    {
    public:
        LookupTree();
        ~LookupTree();
        System::Error insert(Network::IpAddressV4 &address, uint16_t prefix, Network::IpAddressV4 &nextHop);
        System::Error remove(Network::IpAddressV4 &address, uint16_t prefix);
        System::Error getMatchingPrefix(Network::IpAddressV4 &address, Network::IpAddressV4 &result);

    private:
        std::unique_ptr<Node> root;
    };
}

#endif