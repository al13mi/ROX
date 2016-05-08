#ifndef ROX_FEATURESREQUEST_H
#define ROX_FEATURESREQUEST_H
#include "HeaderEncoder.h"
#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class MatchEncoder
        {
        protected:
            uint8_t *p;
        public:
            enum ofp_match_type
            {
                OFPMT_STANDARD = 0,
                OFPMT_OXM = 1,
            };

            explicit MatchEncoder(uint8_t *p_)
            : p(p_) {
            }

            MatchEncoder(MatchEncoder const &encoder) : p(encoder.p)
            {
            }


        };
    }
}
