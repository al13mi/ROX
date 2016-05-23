//
// Created by marnold on 3/19/16.
//

#ifndef ROX_FLOW_MATCH_ENCODER_H
#define ROX_FLOW_MATCH_ENCODER_H

#include "FlowMatchDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FlowMatchEncoder : public FlowMatchDecoder
        {
        public:
            FlowMatchEncoder(uint8_t *p_);
            FlowMatchEncoder(FlowMatchEncoder const &encoder);
            void setFlowMatchType(uint16_t v);
            void setFlowMatchLength(uint16_t v);
        };
    }
}

#endif // ROX_FLOW_MATCH_DECODER_H
