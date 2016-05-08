namespace OpenFlow
{
    namespace Messages
    {
        /**
         * TLV Header:
         * 31-16:  OXM Class
         * 15-9:   OXM Field
         * 8:      HM
         * 7-0:    OXM Length
         */
        class OxmTLV {
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

            void setIPv4FlowMatch(uint32_t inPort, uint8_t* ethDst, uint8_t* ethSrc,
                                  uint32_t ipSrc, uint32_t ipDst, unt16_t srcPort,
                                  uint16_t dstPort);

        private:
            struct OxmMatchFields
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
        };
    }
}