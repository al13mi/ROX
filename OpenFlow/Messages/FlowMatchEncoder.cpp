#include "FlowMatchEncoder.h"
#include <arpa/inet.h>

namespace OpenFlow
{
    namespace Messages
    {

        FlowMatchEncoder::FlowMatchEncoder(uint8_t *p_)
                    : FlowMatchDecoder(p_)
        {
        }

        FlowMatchEncoder::FlowMatchEncoder(FlowMatchEncoder const &encoder) : FlowMatchDecoder(encoder)
        {
        }

        void FlowMatchEncoder::setFlowMatchType(uint16_t v)
        {
            *(uint16_t*)p = ntohs(v);
        }

        void FlowMatchEncoder::setFlowMatchLength(uint16_t v)
        {
            *(uint16_t*)(p+2) = htons(v);
        }
    }
}

