#ifndef IP_ADDRESS_V4_H
#define IP_ADDRESS_V4_H

union IpAddressV4
{
    uint32_t word;
    uint8_t byte[4];
};
#endif