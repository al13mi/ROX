//
// Created by marnold on 3/19/16.
//

#ifndef ROX_FLOW_MATCH_ENCODER_H
#define ROX_FLOW_MATCH_ENCODER_H


#include "Network/Endian.h"
#include "FlowMatchDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FlowMatchEncoder : public FlowMatchDecoder
        {
        public:
            enum ofp_match_type {
                OFPMT_STANDARD = 0,
                OFPMT_OXM = 1,
            };


            explicit FlowMatchEncoder(uint8_t *p_)
                    : FlowMatchDecoder(p_)
            {
            }

            FlowMatchEncoder(FlowMatchEncoder const &encoder) : FlowMatchDecoder(encoder)
            {
            }

            void setFlowMatchType(uint16_t v)
            {
                *(uint16_t*)p = ntohs(v);
            }

            void setFlowMatchLength(uint16_t v)
            {
                *(uint16_t*)(p+2) = htons(v);
            }
        };
    }
}

#endif // ROX_FLOW_MATCH_DECODER_H
