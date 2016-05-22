#include "LookupTree.h"
#include <cassert>
#include <iostream>

namespace System
{
    Node::Node()
       : left(std::unique_ptr<Node>(nullptr))
       , right(std::unique_ptr<Node>(nullptr))
    {
    }

    Node::Node(const Network::IpAddressV4 &address)
        : left(std::unique_ptr<Node>(nullptr))
        , right(std::unique_ptr<Node>(nullptr))
        , nextHop(address.data.word)
    {
    }

    Node::~Node()
    {
    }

    LookupTree::LookupTree()
        : root(new Node())
    {
    }

    LookupTree::~LookupTree()
    {
    }

    // TODO: figure out how to use lambda functions to simplify the crap out of these.
    System::Error LookupTree::insert(Network::IpAddressV4 &address, uint16_t prefix, Network::IpAddressV4 &nextHop)
    {
        Node* current = root.get();

        for(int n = address.BITSET_SIZE-1; n >= 32-prefix; n--)
        {
            if(address.getBitset().test(n))
            {
                if(current->left)
                {
                    current = current->left.get();
                }
                else
                {
                    std::unique_ptr<Node> temp(new Node());
                    current->left = std::move(temp);
                    current = current->left.get();
                }
            }
            else
            {
                if(current->right)
                {
                    current = current->right.get();
                }
                else
                {
                    std::unique_ptr<Node> temp(new Node());
                    current->right = std::move(temp);
                    current = current->right.get();
                }
            }
        }

        current->nextHop.data.word = nextHop.data.word;
        return System::Error::NO_ERROR;
    }

    System::Error LookupTree::remove(Network::IpAddressV4 &address, uint16_t prefix)
    {
        Node* current = root.get();

        for(int n = address.BITSET_SIZE-1; n >= 32-prefix; n--)
        {
            if(address.getBitset().test(n))
            {
                if(current->left)
                {
                    current = current->left.get();
                }
            }
            else
            {
                if(current->right)
                {
                    current = current->right.get();
                }
            }
        }

        current->nextHop.data.word = 0;
        return System::Error::NO_ERROR;
    }

    System::Error LookupTree::getMatchingPrefix(Network::IpAddressV4 &address, Network::IpAddressV4 &result)
    {
        Node* current = root.get();
        uint32_t lastAddress = 0;

        for(int n = address.BITSET_SIZE-1; n >= 0; n--)
        {
            if(address.getBitset().test(n))
            {
                if(current->left)
                {
                    current = current->left.get();
                    if(current->nextHop.data.word != 0)
                    {
                        lastAddress = current->nextHop.data.word;
                    }
                }
            }
            else
            {
                if(current->right)
                {
                    current = current->right.get();
                    if(current->nextHop.data.word != 0)
                    {
                        lastAddress = current->nextHop.data.word;
                    }
                }
            }
        }

        if(lastAddress != 0)
        {
            result.data.word = lastAddress;
            return System::Error::NO_ERROR;
        }
        else
        {
            return System::Error::ERR_ROUTE_NOT_FOUND;
        }
    }
}
