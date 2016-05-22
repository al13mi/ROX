#ifndef ROX_OXMTLV_DECODER_H
#define ROX_OXMTLV_DECODER_H

#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class OxmTLV
        {

        protected:
            uint8_t *p;
        public:


            enum ofp_oxm_class {
                OFPXMC_NXM_0 = 0x0000,
                OFPXMC_NXM_1 = 0x0001,
                OFPXMC_OPENFLOW_BASIC = 0x8000,
                OFPXMC_EXPERIMENTER = 0xFFFF,
            };

            enum oxm_ofb_match_fields {
                OFPXMT_OFB_IN_PORT = 0,
                OFPXMT_OFB_IN_PHY_PORT = 1,
                OFPXMT_OFB_METADATA = 2,
                OFPXMT_OFB_ETH_DST = 3,
                OFPXMT_OFB_ETH_SRC = 4,
                OFPXMT_OFB_ETH_TYPE = 5,
                OFPXMT_OFB_VLAN_VID = 6,
                OFPXMT_OFB_VLAN_PCP = 7,
                OFPXMT_OFB_IP_DSCP = 8,
                OFPXMT_OFB_IP_ECN = 9,
                OFPXMT_OFB_IP_PROTO = 10,
                OFPXMT_OFB_IPV4_SRC = 11,
                OFPXMT_OFB_IPV4_DST = 12,
                OFPXMT_OFB_TCP_SRC = 13,
                OFPXMT_OFB_TCP_DST = 14,
                OFPXMT_OFB_UDP_SRC = 15,
                OFPXMT_OFB_UDP_DST = 16,
                OFPXMT_OFB_SCTP_SRC = 17,
                OFPXMT_OFB_SCTP_DST = 18,
                OFPXMT_OFB_ICMPV4_TYPE = 19,
                OFPXMT_OFB_ICMPV4_CODE = 20,
                OFPXMT_OFB_ARP_OP = 21,
                OFPXMT_OFB_ARP_SPA = 22,
                OFPXMT_OFB_ARP_TPA = 23,
                OFPXMT_OFB_ARP_SHA = 24,
                OFPXMT_OFB_ARP_THA = 25,
                OFPXMT_OFB_IPV6_SRC = 26,
                OFPXMT_OFB_IPV6_DST = 27,
                OFPXMT_OFB_IPV6_FLABEL = 28,
                OFPXMT_OFB_ICMPV6_TYPE = 29,
                OFPXMT_OFB_ICMPV6_CODE = 30,
                OFPXMT_OFB_IPV6_ND_TARGET = 31,
                OFPXMT_OFB_IPV6_ND_SLL = 32,
                OFPXMT_OFB_IPV6_ND_TLL = 33,
                OFPXMT_OFB_MPLS_LABEL = 34,
                OFPXMT_OFB_MPLS_TC = 35,
                OFPXMT_OFP_MPLS_BOS = 36,
                OFPXMT_OFB_PBB_ISID = 37,
                OFPXMT_OFB_TUNNEL_ID = 38,
                OFPXMT_OFB_IPV6_EXTHDR = 39,
                OFPXMT_OFB_PBB_UCA = 41,
            };

            union OxmMatchFields
            {
                uint32_t inPort;
                uint8_t ethDst[12];
                uint8_t ethSrc[12];
                uint16_t ethType;

                uint8_t ipProto;
                uint32_t ipSrc;
                uint32_t ipDst;

                uint16_t srcPort;
                uint16_t dstPort;
            };

            explicit OxmTLV(uint8_t *p_)
            : p(p_)
            {
            }

            OxmTLV(OxmTLV const &decoder)
            {
            }

            uint16_t getOxmClass()
            {
                return ntohs(p[0]);
            }

            void setOxmClass(uint16_t v)
            {
                *(uint16_t*)p = htons(v);
            }

            uint8_t getOxmField()
            {
                return p[2];
            }

            void setOxmField(uint8_t v)
            {
                *(p+2) = v;
            }

            uint8_t getOxmLength()
            {
                return p[3];
            }

            void setOxmLength(uint8_t v)
            {
                *(p+3) = v;
            }

            void setOxmValue(uint32_t v)
            {
                *(uint32_t*)(p+4) = htonl(v);
            }
        };
    }
}

#endif