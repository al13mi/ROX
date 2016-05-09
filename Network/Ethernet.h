#ifndef ETHERNET_H
#define ETHERNET_H
namespace Network {

#pragma pack(1)
    enum Types {
        IPV4 = 0x0800,
        ARP = 0x0806,
        DOT1Q = 0x8100
    };

    struct EthernetHeader {
        uint8_t destMac[6];
        uint8_t sourceMac[6];
        union {
            struct Dot1Q {
                uint8_t tpid[2];
                uint8_t tag[2];
                uint8_t ethertype[2];
                uint8_t payload[1];
            } dot1q;
            struct EtherType {
                uint8_t ethertype[2];
                uint8_t payload[1];
            } type;
        } optional;

        struct IpHeaderV4 {
            uint8_t ipVersionHeaderLen[1];     // IP version & header length (in longs)
            uint8_t tos[1];                    // type of service
            uint8_t ipPacketLength[2];         // total packet length (in octets)
            uint8_t datagramId[2];             // datagram id
            uint8_t fragmentOffset[2];         // fragment offset (in 8-octet's)
            uint8_t ttl[1];                    // time to live, in gateway hops
            uint8_t ipProtocol[1];             // IP protocol (see IP_PROTO_* constants defined above)
            uint8_t checksum[2];               // IP header checksum
            uint8_t source[4];                 // source IP address (network byte order)
            uint8_t destination[4];            // destination IP address (network byte order)
            uint8_t data[1];                   // variable length data
        };
    };
}
#pragma pack()
#endif