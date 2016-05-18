#ifndef IP_ADDRESS_V4_H
#define IP_ADDRESS_V4_H

#include <bitset>

namespace Network {
    struct IpAddressV4 {
        union {
            uint32_t word;
            uint8_t byte[4];
        } data;

        IpAddressV4(uint32_t word) {
            data.word = word;
        }

        ~IpAddressV4() {
        }


        inline std::bitset<32> getBitset() const {
            return std::bitset<32>(data.word);
        }
    };
}

#endif