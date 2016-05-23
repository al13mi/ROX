#ifndef ROX_PACKET_IN_DECODER_H
#define ROX_PACKET_IN_DECODER_H

#include <stdint.h>
#include <iostream>
#include "HeaderDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {
        class PacketInDecoder : public HeaderDecoder
        {
        public:
            PacketInDecoder(uint8_t *p_);
            PacketInDecoder(PacketInDecoder const &decoder);
            uint32_t getBufferId();
            uint16_t getTotalLength();
            uint8_t getReason();
            uint8_t getTableId();
            uint64_t getCookie();
            uint8_t* getOxmMatchHeader();
            uint8_t* getEthernetHeader();
        };
    }
}
#endif
