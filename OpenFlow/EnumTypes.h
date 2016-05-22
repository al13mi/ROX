#ifndef ROX_OPENFLOW_ENUM_TYPES_H
#define ROX_OPENFLOW_ENUM_TYPES_H
enum {
    OFPC_FLOW_STATS = 1 << 0,
    OFPC_TABLE_STATS = 1 << 1,
    OFPC_PORT_STATS = 1 << 2,
    OFPC_GROUP_STATS = 1 << 3,
    OFPC_IP_REASM = 1 << 5,
    OFPC_QUEUE_STATS = 1 << 6,
    OFPC_PORT_BLOCKED = 1 << 8,
    OFPMT_STANDARD = 0,
    OFPMT_OXM = 1,
    OFPAT_OUTPUT = 0,
    OFPAT_COPY_TTL_OUT = 11,
    OFPAT_COPY_TTL_IN = 12,
    OFPAT_SET_MPLS_TTL = 15,
    OFPAT_DEC_MPLS_TTL = 16,
    OFPAT_PUSH_VLAN = 17,
    OFPAT_POP_VLAN = 18,
    OFPAT_PUSH_MPLS = 19,
    OFPAT_POP_MPLS = 20,
    OFPAT_SET_QUEUE = 21,
    OFPAT_GROUP = 22,
    OFPAT_SET_NW_TTL = 23,
    OFPAT_DEC_NW_TTL = 24,
    OFPAT_SET_FIELD = 25,
    OFPAT_PUSH_PBB = 26,
    OFPAT_POP_PBB = 27,
    OFPAT_EXPERIMENTER = 0xffff,
    OFPFC_ADD = 0,
    OFPFC_MODIFY = 1,
    OFPFC_MODIFY_STRICT = 2,
    OFPFC_DELETE = 3,
    OFPFC_DELETE_STRICT = 4,
    OFPP_MAX = 0xffffff00,
    OFPP_IN_PORT = 0xfffffff8,
    OFPP_TABLE = 0xfffffff9,
    OFPP_NORMAL = 0xfffffffa,
    OFPP_FLOOD = 0xfffffffb,
    OFPP_CONTROLLER = 0xfffffffd,
    OFPP_LOCAL = 0xfffffffe,
    OFPP_ANY = 0xffffffff,
    OFPIT_GOTO_TABLE = 1,
    OFPIT_WRITE_METADATA = 2,
    OFPIT_WRITE_ACTIONS = 3,
    OFPIT_APPLY_ACTIONS = 4,
    OFPIT_CLEAR_ACTIONS = 5,
    OFPIT_METER = 6,
    OFPIT_EXPERIMENTER = 0xFFFF
    OFPT_HELLO = 0,
    OFPT_ERROR = 1,
    OFPT_ECHO_REQUEST = 2,
    OFPT_ECHO_REPLY = 3,
    OFPT_EXPERIMENTER = 4,
    OFPT_FEATURES_REQUEST = 5,
    OFPT_FEATURES_REPLY = 6,
    OFPT_GET_CONFIG_REQUEST = 7,
    OFPT_GET_CONFIG_REPLY = 8,
    OFPT_SET_CONFIG = 9,
    OFPT_PACKET_IN = 10,
    OFPT_FLOW_REMOVED = 11,
    OFPT_PORT_STATUS = 12,
    OFPT_PACKET_OUT = 13,
    OFPT_FLOW_MOD = 14,
    OFPT_GROUP_MOD = 15,
    OFPT_PORT_MOD = 16,
    OFPT_TABLE_MOD = 17,
    OFPT_MULTIPART_REQUEST = 18,
    OFPT_MULTIPART_REPLY = 19,
    OFPT_BARRIER_REQUEST = 20,
    OFPT_BARRIER_REPLY = 21,
    OFPT_ROLE_REQUEST = 24,
    OFPT_ROLE_REPLY = 25,
    OFPT_GET_ASYNC_REQUEST = 26,
    OFPT_GET_ASYNC_REPLY = 27,
    OFPT_SET_ASYNC = 28,
    OFPT_METER_MOD = 29,
    OFPT_ROLE_STATUS = 30,
    OFPT_TABLE_STATUS = 31,
    OFPT_REQUESTFORWARD = 32,
    OFPT_BUNDLE_CONTROL = 33,
    OFPT_BUNDLE_ADD_MESSAGE = 34,
    OFPHET_VERSIONBITMAP = 1,
    OFPXMC_NXM_0 = 0x0000,
    OFPXMC_NXM_1 = 0x0001,
    OFPXMC_OPENFLOW_BASIC = 0x8000,
    OFPXMC_EXPERIMENTER = 0xFFFF,
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
    OFPXMT_OFB_PBB_UCA = 41
};
#endif