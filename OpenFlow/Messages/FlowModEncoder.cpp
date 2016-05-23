#include "FlowModEncoder.h"
#include <arpa/inet.h>

namespace OpenFlow
{
    namespace Messages
    {
        FlowModEncoder::FlowModEncoder(uint8_t *p_)
                : HeaderEncoder(p_)
        {
        }

        FlowModEncoder::FlowModEncoder(FlowModEncoder const &decoder) : HeaderEncoder(decoder)
        {
        }

        void FlowModEncoder::setCookie(uint64_t v)
        {
            *(uint64_t*)(p+HEADER_MINIMUM_LENGTH) = htobe64(v);
        }

        void FlowModEncoder::setCookieMask(uint64_t v)
        {
            *(uint64_t*)(p+HEADER_MINIMUM_LENGTH + 8) = htobe64(v);
        }

        void FlowModEncoder::setTableId(uint8_t v)
        {
            p[HEADER_MINIMUM_LENGTH + 8 + 8] = v;
        }

        void FlowModEncoder::setCommand(uint8_t v)
        {
            p[HEADER_MINIMUM_LENGTH + 8 + 8 + 1] = v;
        }

        void FlowModEncoder::setIdleTimeout(uint16_t v)
        {
            *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 2) = htons(v);
        }

        void FlowModEncoder::setHardTimeout(uint16_t v)
        {
            *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 4) = htons(v);
        }

        void FlowModEncoder::setPriority(uint16_t v)
        {
            *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 6) = htons(v);
        }

        void FlowModEncoder::setBufferId(uint32_t v)
        {
            *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8) = htonl(v);
        }

        void FlowModEncoder::setOutPort(uint32_t v)
        {
            *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 4) = htonl(v);
        }

        void FlowModEncoder::setOutGroup(uint32_t v)
        {
            *(uint32_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8) = htonl(v);
        }

        void FlowModEncoder::setFlags(uint16_t v)
        {
            *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 4) = htons(v);
        }

        void FlowModEncoder::setImportance(uint16_t v)
        {
            *(uint16_t*)(p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 6) = htons(v);
        }

        uint8_t* FlowModEncoder::getMatchFieldWritePtr()
        {
            return p+HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8;
        }

        void FlowModEncoder::padMatchField()
        {

            //p[HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 8] = htons(0); // type
            //p[HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 8 + 2] = htons(0); // length
            //p[HEADER_MINIMUM_LENGTH + 8 + 8 + 8 + 8 + 8 + 8 + 2 + 2] = htonl(0); // padding
        }

        uint16_t FlowModEncoder::getFlowModMinLength()
        {
            return 56;
        }
    }
}
