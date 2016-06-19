//
// Created by marnold on 3/19/16.
//

#ifndef ROX_FLOW_REMOVED_DECODER_H
#define ROX_FLOW_REMOVED_DECODER_H

#include <stdint.h>
#include "HeaderDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FlowRemovedDecoder : public HeaderDecoder
        {
        public:
            FlowRemovedDecoder(uint8_t *p_);
            FlowRemovedDecoder(FlowRemovedDecoder const &decoder);
            uint64_t getCookie() const;
            uint16_t getPriority() const;
            uint8_t getReason() const;
            uint8_t getTableId() const;
            uint32_t getDurationSec() const;
            uint32_t getDurationNSec() const;
            uint16_t getIdleTimeout() const;
            uint16_t getHardTimeout() const;

            uint64_t getPacketCount() const;
            uint64_t getByteCount() const;
        };
    }
}

#endif //ROX_HEADERDECODER_H
