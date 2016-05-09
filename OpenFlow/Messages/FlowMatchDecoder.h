//
// Created by marnold on 3/19/16.
//

#ifndef ROX_FLOW_MATCH_DECODER_H
#define ROX_FLOW_MATCH_DECODER_H


#include "Network/Endian.h"
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
            enum ofp_match_type {
                OFPMT_STANDARD = 0,
                OFPMT_OXM = 1,
            };


            explicit FlowMatchDecoder(uint8_t *p_)
                    : p(p_)
            {
            }

            FlowMatchDecoder(FlowMatchDecoder const &decoder)
            {
            }

            uint16_t getFlowMatchType()
            {
                uint16_t *temp = (uint16_t*)(p);
                return ntohs(*temp);
            }

            uint16_t getFlowMatchLength()
            {
                uint16_t *temp = (uint16_t*)(p+2);
                return ntohs(*temp);
            }

            uint8_t* getOxmFields()
            {
                return p + 4;
            }

            uint16_t getPadding()
            {
                return 16 % getFlowMatchLength();
            }


        };
    }
}

#endif // ROX_FLOW_MATCH_DECODER_H
