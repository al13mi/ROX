#ifndef ROX_HEADERENCODER_H
#define ROX_HEADERENCODER_H

#include "HeaderDecoder.h"
#include <stdint.h>

namespace OpenFlow
{
    namespace Messages
    {
        class HeaderEncoder : public HeaderDecoder
        {
        public:
            explicit HeaderEncoder(uint8_t *p_);
            HeaderEncoder(HeaderEncoder const &encoder);
            void setVersion(uint8_t v);
            void setType(uint8_t v);
            void setLength(uint16_t v);
            void setXid(uint32_t v) const;
        };
    }
}
#endif // ROX_HEADERENCODER_H