#include "FlowModInstructionEncoder.h"
#include <arpa/inet.h>

namespace OpenFlow
{
    namespace Messages
    {

         FlowModInstructionEncoder::FlowModInstructionEncoder(uint8_t *p_)
                : p(p_)
        {
        }

         void  FlowModInstructionEncoder::setType(uint16_t v)
        {
            *(uint16_t*)p = htons(v);
        }

         void  FlowModInstructionEncoder::setLength(uint16_t v)
        {
            *(uint16_t*)(p+2) = htons(v);
        }

         void  FlowModInstructionEncoder::setPadding()
        {
            *(uint32_t*)(p+4) = 0;
        }

         uint16_t  FlowModInstructionEncoder::getLength()
        {
            return 8;
        }
    }
}
