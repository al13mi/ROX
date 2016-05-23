#include <stdint.h>
#include <arpa/inet.h>

#include "HeaderDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {

        HeaderDecoder::HeaderDecoder(uint8_t *p_)
                : p(p_) {
        }

        HeaderDecoder::HeaderDecoder(HeaderDecoder const &decoder) : p(decoder.p)
        {
        }

        uint8_t HeaderDecoder::getVersion() const
        {
            return p[0];
        }

        uint8_t HeaderDecoder::getType() const
        {
            return p[1];
        }

        uint16_t HeaderDecoder::getLength() const
        {
            uint16_t *temp = (uint16_t*)(p+2);
            return ntohs(*temp);
        }

        uint32_t HeaderDecoder::getXid() const
        {
            uint32_t *temp = (uint32_t*)(p+4);
            return ntohl(*temp);
        }

        uint8_t* HeaderDecoder::getReadPtr()
        {
            return p;
        }
    }
}

