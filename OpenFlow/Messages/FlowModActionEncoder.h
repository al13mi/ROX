#ifndef ROX_MATCH_ACTION_ENCODER_H
#define ROX_MATCH_ACTION_ENCODER_H
#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FlowModActionEncoder
        {
        private:
            uint8_t *p;
        public:
            enum ofp_action_type {
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
                OFPAT_EXPERIMENTER = 0xffff
            };

            explicit FlowModActionEncoder(uint8_t *p_)
                    : p(p_)
            {
            }

            void setType(uint16_t v)
            {
                *(uint16_t*)p = htons(v);
            }

            void setActionLen(uint16_t v)
            {
                *(uint16_t*)(p+2) = htons(v);
            }

            void setOutputPort(uint32_t v)
            {
                *(uint32_t*)(p+4) = htonl(v);
            }

            void setMaxLen(uint16_t v)
            {
                *(uint16_t*)(p+4+4) = htons(v);
            }

            void setPadding()
            {
                *(uint32_t*)(p+8+2) = 0;
                *(uint16_t*)(p+8+2 +4) = 0;
            }

            uint16_t getLength()
            {
                return 16;
            }
        };
    }
}
#endif