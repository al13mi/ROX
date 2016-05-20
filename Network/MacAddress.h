#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H


namespace Network {
    struct MacAddress {
        union {
            uint64_t word;
            uint8_t byte[6];

        } data;

        MacAddress(const uint8_t * bytes)
        {
            for(int n = 0; n<6; n++)
            {
                data.byte[n] = bytes[n];
            }
        }
    };
}

#endif