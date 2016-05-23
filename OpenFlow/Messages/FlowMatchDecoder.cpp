#include "FlowMatchDecoder.h"
#include <arpa/inet.h>

namespace OpenFlow
{
    namespace Messages
    {
        FlowMatchDecoder::FlowMatchDecoder(uint8_t *p_) : p(p_)
        {
        }

        FlowMatchDecoder::FlowMatchDecoder(FlowMatchDecoder const &decoder)
        {
        }

        uint16_t FlowMatchDecoder::getFlowMatchType()
        {
            uint16_t *temp = (uint16_t*)(p);
            return ntohs(*temp);
        }

        uint16_t FlowMatchDecoder::getFlowMatchLength()
        {
            uint16_t *temp = (uint16_t*)(p+2);
            return ntohs(*temp);
        }

        uint8_t* FlowMatchDecoder::getOxmFields()
        {
            return p + 4;
        }

        uint16_t FlowMatchDecoder::getPadding()
        {
            return 16 % getFlowMatchLength();
        }
    }
}
