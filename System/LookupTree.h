#ifndef TREE_H
#define TREE_H

#include <map>
#include "../Network/IpAddressV4.h"
#include "Error.h"

namespace System
{
    struct Node
    {
        Node();
        Node(const Network::IpAddressV4  &address);
        Node(const Node &node);
        ~Node();

        Node *left;
        Node *right;
        Network::IpAddressV4 *nextHop;
        bool end;
    };

    class LookupTree
    {
    public:
        LookupTree();
        System::Error insert(Node &node);
        System::Error remove(Node &node);
        System::Error getMatchingPrefix(Network::IpAddressV4 &nextHop, Network::IpAddressV4 &address);

    private:
        Node root;
    };
}

#endif