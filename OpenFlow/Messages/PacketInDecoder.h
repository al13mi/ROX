#ifndef ROX_PACKET_IN_DECODER_H
#define ROX_PACKET_IN_DECODER_H
#include "HeaderDecoder.h"
#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class PacketInDecoder : public HeaderDecoder
        {
        protected:
            uint8_t *p;
        public:
            enum ofp_packet_in_reason {
                OFPR_TABLE_MISS = 0,
                OFPR_APPLY_ACTION = 1,
                OFPR_INVALID_TTL = 2,
                OFPR_ACTION_SET = 3,
                OFPR_GROUP = 4,
                OFPR_PACKET_OUT = 5,
            };

            explicit PacketInDecoder(uint8_t *p_)
                    : HeaderDecoder(p_)
            {
            }

            PacketInDecoder(PacketInDecoder const &decoder) : HeaderDecoder(decoder)
            {
            }
        };
    }
}
#endif