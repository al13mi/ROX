//
// Created by marnold on 3/19/16.
//

#ifndef ROX_FLOW_MATCH_DECODER_H
#define ROX_FLOW_MATCH_DECODER_H


#include "HeaderDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FlowMatchDecoder
        {
        protected:
            uint8_t *p;
        public:
            FlowMatchDecoder(uint8_t *p_);
            FlowMatchDecoder(FlowMatchDecoder const &decoder);
            uint16_t getFlowMatchType();
            uint16_t getFlowMatchLength();
            uint8_t* getOxmFields();
            uint16_t getPadding();
        };
    }
}

#endif // ROX_FLOW_MATCH_DECODER_H
