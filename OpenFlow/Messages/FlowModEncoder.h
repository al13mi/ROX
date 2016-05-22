#ifndef ROX_FLOW_MOD_ENCODER_H
#define ROX_FLOW_MOD_ENCODER_H
#include "HeaderEncoder.h"
#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FlowModEncoder : public HeaderEncoder
        {
        public:
            enum ofp_flow_mod_command
            {
                OFPFC_ADD = 0,
                OFPFC_MODIFY = 1,
                OFPFC_MODIFY_STRICT = 2,
                OFPFC_DELETE = 3,
                OFPFC_DELETE_STRICT = 4,
            };

            enum ofp_port_no
            {
                OFPP_MAX = 0xffffff00,
                OFPP_IN_PORT = 0xfffffff8,
                OFPP_TABLE = 0xfffffff9,
                OFPP_NORMAL = 0xfffffffa,
                OFPP_FLOOD = 0xfffffffb,
                OFPP_CONTROLLER = 0xfffffffd,
                OFPP_LOCAL = 0xfffffffe,
                OFPP_ANY = 0xffffffff
            };

            enum ofp_group {
                OFPG_MAX = 0xffffff00,
                OFPG_ALL = 0xfffffffc,
                OFPG_ANY = 0xffffffff
            };


            explicit FlowModEncoder(uint8_t *p_)
                : HeaderEncoder(p_)
            {
            }

            FlowModEncoder(FlowModEncoder const &decoder) : HeaderEncoder(decoder)
            {
            }

            void setCookie(uint64_t v)
            {
                *(uint64_t*)(p+HEADER_MINIMUM_LENGTH) = htonll(v);
            }

            void setCookieMask(uint64_t v)
            {
                *(uint64_t*)(p+HEADER_MINIMUM_LENGTH + 8) = htonll(v);
            }

            void setTableId(uint8_t v)
            {
                p[HEADER_MINIMUM_LENGTH + 8 + 8] = v;
            }

            void setCommand(uint8_t v)
            {
                p[HEADER_MINIMUM_LENGTH + 8 + 8 + 1] = v;
            }

            void setIdleTimeout(uint16_t v)
            {
                *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 2) = htons(v);
            }

            void setHardTimeout(uint16_t v)
            {
                *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 4) = htons(v);
            }

            void setPriority(uint16_t v)
            {
                *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 6) = htons(v);
            }

            void setBufferId(uint32_t v)
            {
                *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8) = htonl(v);
            }

            void setOutPort(uint32_t v)
            {
                *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 4) = htonl(v);
            }

            void setOutGroup(uint32_t v)
            {
                *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8) = htonl(v);
            }

            void setFlags(uint16_t v)
            {
                *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 4) = htons(v);
            }

            void setImportance(uint16_t v)
            {
                *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 6) = htons(v);
            }

            uint8_t *getMatchFieldWritePtr()
            {
                return p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8;
            }

            void padMatchField()
            {

                //p[HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 8] = htons(0); // type
                //p[HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 8 + 2] = htons(0); // length
                //p[HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 8 + 2 + 2] = htonl(0); // padding
            }

            uint16_t getFlowModMinLength()
            {
                return 56;
            }
        };
    }
}
#endif