#include "LookupTree.h"

namespace System
{
    Node::Node()
        : left(NULL)
        , right(NULL)
        , nextHop(NULL)
    {

    }

    Node::Node(const Network::IpAddressV4 &address)
    {

    }

    Node::Node(const Node &node)
    {

    }

    Node::~Node()
    {

    }

    LookupTree::LookupTree()
    {

    }

    System::Error LookupTree::insert(Node &node)
    {
        return System::Error::NO_ERROR;
    }

    System::Error LookupTree::remove(Node &node)
    {
        return System::Error::NO_ERROR;
    }

    System::Error LookupTree::getMatchingPrefix(Network::IpAddressV4 &nextHop, Network::IpAddressV4 &address)
    {
        return System::Error::NO_ERROR;
    }

}
