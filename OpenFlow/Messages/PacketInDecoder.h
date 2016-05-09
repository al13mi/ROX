#ifndef ROX_PACKET_IN_DECODER_H
#define ROX_PACKET_IN_DECODER_H

#include "HeaderDecoder.h"
#include "Network/Endian.h"

#include <iostream>

namespace OpenFlow
{
    namespace Messages
    {
        class PacketInDecoder : public HeaderDecoder
        {
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

            uint32_t getBufferId()
            {
                uint32_t *temp = (uint32_t*)(p+HEADER_MINIMUM_LENGTH);
                return ntohl(*temp);
            }

            uint16_t getTotalLength()
            {
                uint16_t *temp = (uint16_t*)(p+HEADER_MINIMUM_LENGTH+4);
                return ntohs(*temp);
            }

            uint8_t getReason()
            {
                return p[HEADER_MINIMUM_LENGTH+6];
            }

            uint8_t getTableId()
            {
                return p[HEADER_MINIMUM_LENGTH + 7];
            }

            uint64_t getCookie()
            {
                uint64_t *temp = (uint64_t*)(p+HEADER_MINIMUM_LENGTH+8);
                return ntohll(*temp);
            }

            uint8_t* getOxmMatchHeader()
            {

                return p + HEADER_MINIMUM_LENGTH + 16;
            }

            uint8_t* getEthernetHeader()
            {
                FlowMatchDecoder match(getOxmMatchHeader());
                uint16_t len = match.getFlowMatchLength();
                return getOxmMatchHeader() + len + match.getPadding() + 2;
            }
        };
    }
}
#endif