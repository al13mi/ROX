#ifndef TREE_H
#define TREE_H

#include <map>
#include "IpAddressV4.h"

namespace System
{
    struct Node
    {
        Node();
        Node(const IpAddressV4  &address);
        Node(const Node &node);
        ~Node();

        uint8_t octet;
        std::map <uint8_t, Node> children;
        bool end;
    };

    class LookupTree
    {
    public:
        LookupTree();
        void insert(Node &node);

        void getMatchingPrefix(IpAddressV4 &address);

    private:
        Node root;
    };
}

#endif