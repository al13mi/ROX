//
// Created by marnold on 3/19/16.
//

#ifndef ROX_HEADERDECODER_H
#define ROX_HEADERDECODER_H

#include <stdint.h>

namespace OpenFlow
{
    namespace Messages
    {
        class HeaderDecoder
        {
        protected:
            uint8_t *p;
        public:
            enum { HEADER_MINIMUM_LENGTH = 8};

            HeaderDecoder(uint8_t *p_);
            HeaderDecoder(HeaderDecoder const &decoder);
            uint8_t getVersion() const;
            uint8_t getType() const;
            uint16_t getLength() const;
            uint32_t getXid() const;
            uint8_t *getReadPtr();
        };
    }
}

#endif //ROX_HEADERDECODER_H
