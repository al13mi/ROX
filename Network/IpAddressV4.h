#ifndef IP_ADDRESS_V4_H
#define IP_ADDRESS_V4_H

#include <bitset>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

namespace Network {
    struct IpAddressV4 {
        enum
        {
            BITSET_SIZE = 32,
        };
        union {
            uint32_t word;
            uint8_t byte[4];
        } data;

        IpAddressV4(uint32_t word) {
            data.word = word;
        }

        IpAddressV4()
        {
            data.word = 0;
        }

        IpAddressV4(const char* address)
        {
            std::string temp = address;
            std::vector<std::string> tokens;
            std::string item;
            std::stringstream ss(temp);

            while(std::getline(ss, item, '.'))
            {
                tokens.push_back(item);
            }

            for(unsigned i = 0; i < 4; ++i)
            {
                data.word += std::stoul(tokens[i]) << 8;
            }
        }

        ~IpAddressV4() {
        }

        inline std::bitset<BITSET_SIZE> getBitset() const {
            return std::bitset<BITSET_SIZE>(data.word);
        }
    };
}

#endif