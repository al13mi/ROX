//
// Created by marnold on 3/19/16.
//

#ifndef ROX_HELLODECODER_H
#define ROX_HELLODECODER_H

#include <stdint.h>
#include "HeaderDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {
        class HelloDecoder : public HeaderDecoder
        {
        public:
            enum
            {
                HELLO_ELEMENT_SIZE = 4
            };

            HelloDecoder(uint8_t *p_);
            HelloDecoder(HelloDecoder const &decoder);
            uint16_t getNumHelloElements();
            uint8_t *getHelloElement(int num);
        };
    }
}

#endif //ROX_HELLODECODER_H
