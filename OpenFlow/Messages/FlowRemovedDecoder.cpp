#include <stdint.h>
#include <arpa/inet.h>

#include "FlowRemovedDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {

        FlowRemovedDecoder::FlowRemovedDecoder(uint8_t *p_)
                : HeaderDecoder(p_) {
        }

        FlowRemovedDecoder::FlowRemovedDecoder(FlowRemovedDecoder const &decoder) : HeaderDecoder(decoder.p)
        {
        }

        uint64_t FlowRemovedDecoder::getCookie() const
        {
            uint64_t *temp = (uint64_t * )(p + HEADER_MINIMUM_LENGTH);
            return htobe64(*temp);
        }

        uint16_t FlowRemovedDecoder::getPriority() const
        {
            uint16_t *temp = (uint16_t * )(p + HEADER_MINIMUM_LENGTH + 8);
            return ntohs(*temp);
        }

        uint8_t FlowRemovedDecoder::getReason() const
        {
            return p[HEADER_MINIMUM_LENGTH + 8 + 2];
        }

        uint8_t FlowRemovedDecoder::getTableId() const
        {
            return p[HEADER_MINIMUM_LENGTH + 8 + 1 + 2];
        }

        uint32_t FlowRemovedDecoder::getDurationSec() const
        {
            uint32_t *temp = (uint32_t * )(p + HEADER_MINIMUM_LENGTH + 8 + 2 + 2);
            return ntohl(*temp);
        }

        uint32_t FlowRemovedDecoder::getDurationNSec() const
        {
            uint32_t *temp = (uint32_t * )(p + HEADER_MINIMUM_LENGTH + 8 + 6 + 2);
            return ntohl(*temp);
        }

        uint16_t FlowRemovedDecoder::getIdleTimeout() const
        {
            uint16_t *temp = (uint16_t * )(p + HEADER_MINIMUM_LENGTH + 8 + 10 + 2);
            return ntohs(*temp);
        }

        uint16_t FlowRemovedDecoder::getHardTimeout() const
        {
            uint16_t *temp = (uint16_t * )(p + HEADER_MINIMUM_LENGTH + 8 + 12 + 2);
            return ntohs(*temp);
        }

        uint64_t FlowRemovedDecoder::getPacketCount() const
        {
            uint64_t *temp = (uint64_t * )(p + HEADER_MINIMUM_LENGTH + 8 + 14 + 2);
            return htobe64(*temp);
        }

        uint64_t FlowRemovedDecoder::getByteCount() const
        {
            uint64_t *temp = (uint64_t * )(p + HEADER_MINIMUM_LENGTH + 22 + 8 + 2);
            return htobe64(*temp);
        }
    }
}

