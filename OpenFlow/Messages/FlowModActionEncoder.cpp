#include "FlowModActionEncoder.h"
#include <arpa/inet.h>

namespace OpenFlow
{
    namespace Messages
    {

        FlowModActionEncoder::FlowModActionEncoder(uint8_t *p_)
                : p(p_)
        {
        }

        void FlowModActionEncoder::setType(uint16_t v)
        {
            *(uint16_t*)p = htons(v);
        }

        void FlowModActionEncoder::setActionLen(uint16_t v)
        {
            *(uint16_t*)(p+2) = htons(v);
        }

        void FlowModActionEncoder::setOutputPort(uint32_t v)
        {
            *(uint32_t*)(p+4) = htonl(v);
        }

        void FlowModActionEncoder::setMaxLen(uint16_t v)
        {
            *(uint16_t*)(p+4+4) = htons(v);
        }

        void FlowModActionEncoder::setPadding()
        {
            *(uint32_t*)(p+8+2) = 0;
            *(uint16_t*)(p+8+2 +4) = 0;
        }

        uint16_t FlowModActionEncoder::getLength()
        {
            return 16;
        }
    }
}
