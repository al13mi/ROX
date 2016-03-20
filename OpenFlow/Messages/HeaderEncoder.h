#ifndef ROX_HEADERENCODER_H
#define ROX_HEADERENCODER_H
#include "HeaderDecoder.h"
#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class HeaderEncoder : public HeaderDecoder
        {
        public:
            explicit HeaderEncoder(uint8_t *p_)
                : HeaderDecoder(p_)
            {
            }

            HeaderEncoder(HeaderEncoder const &encoder) : HeaderDecoder(encoder)
            {
            }

            void setVersion(uint8_t v)
            {
                *p = v;
            }

            void setType(uint8_t v)
            {
                p[1] = v;
            }

            void setLength(uint16_t v)
            {
                *(uint16_t*)(p+2) = htons(v);
            }

            void setXid(uint32_t v) const
            {
                *(uint32_t*)(p+4) = htonl(v);
            }
        };
    }
}
#endif // ROX_HEADERENCODER_H