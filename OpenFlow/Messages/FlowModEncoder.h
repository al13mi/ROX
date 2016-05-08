#ifndef ROX_FEATURESREQUEST_H
#define ROX_FEATURESREQUEST_H
#include "HeaderEncoder.h"
#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FlowModEncoder : public HeaderEncoder
        {
        protected:
            uint8_t *p;
        public:
            enum ofp_flow_mod_command
            {
                OFPFC_ADD = 0,
                OFPFC_MODIFY = 1,
                OFPFC_MODIFY_STRICT = 2,
                OFPFC_DELETE = 3,
                OFPFC_DELETE_STRICT = 4,
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
                *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 2) = htonls(v);
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
                *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8) = htonl(v);
            }

            void setOutPort(uint32_t v)
            {
                *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 4) = htonl(v);
            }

            void setOutGroup(uint32_t v)
            {
                *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8) = htonl(v);
            }

            void setFlags(uint16_t v)
            {
                *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 4) = htons(v);
            }

            void setImportance(uint16_t v)
            {
                *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 6) = htons(v);
            }

            uint8_t *getMatchFieldWritePtr()
            {
                return p[HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 8];
            }
        };
    }
}
#endif