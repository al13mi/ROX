#ifndef ETHERNET_H
#define ETHERNET_H

#include <arpa/inet.h>

enum { IP_PROTO_TCP         = 6   };   // protocol type for TCP packets
enum { IP_PROTO_UDP         = 17  };   // protocol type for UDP packets
enum { IP_PROTO_EXP         = 253 };

namespace Network {

#pragma pack(1)

    inline uint32_t getHostMask(uint32_t address, uint32_t size)
    {
        uint32_t mask = 0xFFFFFFFF >> (32 - size);
        mask = mask << (32 - size);
        uint32_t key = mask & address;
        return key;
    }


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

    };

#pragma pack(1)

    struct UdpHeaderV4
    {
        uint8_t sourcePort[2];             // Sender's port number
        uint8_t destinationPort[2];        // Receiver's port number
        uint8_t length[2];                 // UDP header+data length (in octets)
        uint8_t checksum[2];               // UDP header checksum
        uint8_t data[1];                   // variable length data
    };

#pragma pack()

#pragma pack(1)

    struct TcpHeaderV4
    {

        uint8_t sourcePort[2];         // Sender's port number
        uint8_t destinationPort[2];    // Receiver's port number
        uint8_t seqNum[4];             // Sequence number of the first data byte in this segment
        uint8_t ackNum[4];             // Acknowledgement number
        uint8_t offset[1];             //
        uint8_t controlFlags[1];       // Control flags
        uint8_t window[2];             // Current TCP window size
        uint8_t checksum[2];           // TCP header checksum
        uint8_t urgentPointer[2];      // Sequence number of the last byte in a urgent data sequence
        uint8_t data[1];               // variable length data
    };

#pragma pack()

#pragma pack(1)

    struct IcmpHeaderV4
    {
        uint8_t type[1];                      // ICMP message type
        uint8_t code[1];                      // ICMP message code
        uint8_t checksum[2];                  // ICMP checksum
        union
        {
            struct
            {
                uint8_t id[2];                // Echo request/reply identifier
                uint8_t seqNum[2];            // Echo request/reply sequence number
                uint8_t data[1];              // variable length data
            } echo;
            struct
            {
                uint8_t ipAddress[4];         // IP address of the gateway (in network byte order)
                uint8_t  data[1];             // variable length data
            } redirect;
            struct
            {
                uint8_t unused[2];            // Unused field.  Should always be set to zero.
                uint8_t nextHopMtu[2];        // The next-hop network's MTU. Used for Path MTU Discovery (RFC 1192).
                uint8_t  data[1];             // variable length data
            } destUnreachable;
            struct
            {
                uint8_t id[2];                // Identifier
                uint8_t unused[2];            // Unused field.  Should always be set to zero.
                uint8_t outHopCount[2];       // Outbound hop count
                uint8_t returnHopCount[2];    // Return hop count
                uint8_t outLinkSpeed[4];      // Output link speed
                uint8_t outLinkMtu[4];        // Output link MTU
                uint8_t  data[1];             // variable length data
            } traceRoute;
            struct
            {
                uint8_t unused[4];            // Unused field.  Should always be set to zero.
                uint8_t data[1];              // variable length data
            } generic;
            uint8_t data[1];                  // variable length data
        } extendedHeader;
    };

#pragma pack()

#pragma pack(1)

    struct IpHeaderV4
    {
        uint8_t ipVersionHeaderLen[1];    
        uint8_t tos[1];
        uint8_t ipPacketLength[2];
        uint8_t datagramId[2];
        uint8_t fragmentOffset[2];
        uint8_t ttl[1];
        uint8_t ipProtocol[1];
        uint8_t checksum[2];
        uint8_t source[4];
        uint8_t destination[4];
        uint8_t data[1];
};

#pragma pack()


    class FlowIndexV4
    {
    public:
	uint8_t pkt[2000];
        inline uint32_t netByteArrayToU32 (const uint8_t * src)
        {
            uint32_t dest;
            dest  = src[0]<<24;
            dest |= src[1]<<16;
            dest |= src[2]<<8;
            dest |= src[3];
            return dest;
        }

        inline uint32_t netByteArrayToU24 (const uint8_t * src)
        {
            uint32_t dest;
            dest  = src[0]<<16;
            dest |= src[1]<<8;
            dest |= src[2];
            return dest;
        }

        inline uint16_t netByteArrayToU16 (const uint8_t * src)
        {
            uint16_t dest;
            dest = src[0]<<8;
            dest |= src[1];
            return dest;
        }

        enum { IP_TOS_PRECEDENCE_MASK  = 0xE0   };
        enum { IP_TOS_DSCP_MASK        = 0xFC   };
        enum { IP_TOS_ECN_MASK         = 0x03   };
        enum { IP_TOS_PRECEDENCE_SHIFT = 5      };
        enum { IP_TOS_DSCP_SHIFT       = 2      };

        FlowIndexV4() {}

        ~FlowIndexV4() {}

        inline FlowIndexV4(const IpHeaderV4 *ipPkt)
        {
            contents.key[0] = 0;
            contents.ip.protocol = *ipPkt->ipProtocol;
            contents.ip.dscp = (*ipPkt->tos) >> IP_TOS_DSCP_SHIFT;
            contents.ip.destIp = htonl(netByteArrayToU32(ipPkt->destination));
            contents.ip.srcIp =  htonl(netByteArrayToU32(ipPkt->source));

            switch(*ipPkt->ipProtocol)
            {
                case IP_PROTO_UDP:
                {
                    UdpHeaderV4 *udpPkt = (UdpHeaderV4 *)ipPkt->data;
                    contents.ip.protocolSpecific.tcpUdp.destPort =  netByteArrayToU16(udpPkt->destinationPort);
                    contents.ip.protocolSpecific.tcpUdp.srcPort = netByteArrayToU16(udpPkt->sourcePort);
                }
                    break;
                case IP_PROTO_TCP:
                {
                    TcpHeaderV4 *tcpPkt = (TcpHeaderV4 *)ipPkt->data;
                    contents.ip.protocolSpecific.tcpUdp.destPort = netByteArrayToU16(tcpPkt->destinationPort);
                    contents.ip.protocolSpecific.tcpUdp.srcPort = netByteArrayToU16(tcpPkt->sourcePort);
                }
                    break;

                default:
                    contents.ip.protocolSpecific.other.empty = 0;
                    break;
            }
        }

        inline FlowIndexV4(const FlowIndexV4& other)
        {
            contents.key[3] = other.contents.key[3];
            contents.key[2] = other.contents.key[2];
            contents.key[1] = other.contents.key[1];
            contents.key[0] = other.contents.key[0];
        }

        inline bool operator==(const FlowIndexV4 &other) const
        {
            return (contents.key[3] == other.contents.key[3])
                   && (contents.key[2] == other.contents.key[2])
                   && (contents.key[1] == other.contents.key[1])
                   && (contents.key[0] == other.contents.key[0]);
        }

        enum { UINT64_KEY_SIZE = 2 };
        enum { KEY_SIZE = 4 };
        enum { BYTE_KEY_SIZE = 16 };
        union
        {
            struct
            {
                uint8_t  pad[2];
                uint8_t  protocol;
                uint8_t  dscp;
                uint32_t srcIp;
                uint32_t destIp;
                union
                {
                    struct
                    {
                        uint16_t destPort;
                        uint16_t srcPort;
                    } tcpUdp;
                    struct
                    {
                        uint8_t  type;
                        uint8_t  code;
                        uint16_t id;
                    } icmp;
                    struct
                    {
                        uint32_t spi;
                    } ipSec;
                    struct
                    {
                        uint32_t key;
                    } gre;
                    struct
                    {
                        uint32_t empty;
                    } other;
                } protocolSpecific;
            } ip;
            uint64_t       u64Key[UINT64_KEY_SIZE];
            uint32_t       key[KEY_SIZE];
            uint8_t        byteKey[BYTE_KEY_SIZE];
        } contents;
    };


}
#pragma pack()
#endif
