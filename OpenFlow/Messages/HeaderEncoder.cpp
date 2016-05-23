#include "HeaderEncoder.h"
#include <arpa/inet.h>

namespace OpenFlow
{
    namespace Messages
    {

        HeaderEncoder::HeaderEncoder(uint8_t *p_)
                : HeaderDecoder(p_)
        {
        }

        HeaderEncoder::HeaderEncoder(HeaderEncoder const &encoder) : HeaderDecoder(encoder)
        {
        }

        void HeaderEncoder::setVersion(uint8_t v)
        {
            *p = v;
        }

        void HeaderEncoder::setType(uint8_t v)
        {
            p[1] = v;
        }

        void HeaderEncoder::setLength(uint16_t v)
        {
            *(uint16_t*)(p+2) = htons(v);
        }

        void HeaderEncoder::setXid(uint32_t v) const
        {
            *(uint32_t*)(p+4) = htonl(v);
        }
    }
}
