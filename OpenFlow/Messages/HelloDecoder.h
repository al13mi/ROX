//
// Created by marnold on 3/19/16.
//

#ifndef ROX_HELLODECODER_H
#define ROX_HELLODECODER_H


#include "Network/Endian.h"
#include "HeaderDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {
        class HelloDecoder : public HeaderDecoder
        {
        protected:
            uint8_t *p;
        public:
            enum
            {
                OFPHET_VERSIONBITMAP = 1,
                HELLO_ELEMENT_SIZE = 4
            };

            explicit HelloDecoder(uint8_t *p_)
            : HeaderDecoder(p_)
            {
            }

            HelloDecoder(HelloDecoder const &decoder) : HeaderDecoder(decoder)
            {
            }

            uint16_t getNumHelloElements()
            {
                return (getLength() - HEADER_MINIMUM_LENGTH)/ HELLO_ELEMENT_SIZE;
            }

            uint8_t *getHelloElement(int num)
            {
                return &p[HEADER_MINIMUM_LENGTH + num];
            }
        };
    }
}

#endif //ROX_HELLODECODER_H
