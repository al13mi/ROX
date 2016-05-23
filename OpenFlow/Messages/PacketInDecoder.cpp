#include "PacketInDecoder.h"
#include <arpa/inet.h>

#include "OpenFlow/EnumTypes.h"
#include "FlowMatchDecoder.h"
namespace OpenFlow
{
    namespace Messages
    {

        PacketInDecoder::PacketInDecoder(uint8_t *p_)
                : HeaderDecoder(p_)
        {
        }

        PacketInDecoder::PacketInDecoder(PacketInDecoder const &decoder) : HeaderDecoder(decoder)
        {
        }

        uint32_t PacketInDecoder::getBufferId()
        {
            uint32_t *temp = (uint32_t*)(p+HeaderDecoder::HEADER_MINIMUM_LENGTH);
            return ntohl(*temp);
        }

        uint16_t PacketInDecoder::getTotalLength()
        {
            uint16_t *temp = (uint16_t*)(p+HeaderDecoder::HEADER_MINIMUM_LENGTH+4);
            return ntohs(*temp);
        }

        uint8_t PacketInDecoder::getReason()
        {
            return p[HeaderDecoder::HEADER_MINIMUM_LENGTH+6];
        }

        uint8_t PacketInDecoder::getTableId()
        {
            return p[HeaderDecoder::HEADER_MINIMUM_LENGTH + 7];
        }

        uint64_t PacketInDecoder::getCookie()
        {
            uint64_t *temp = (uint64_t*)(p+HeaderDecoder::HEADER_MINIMUM_LENGTH+8);
            return be64toh(*temp);
        }

        uint8_t* PacketInDecoder::getOxmMatchHeader()
        {

            return p + HeaderDecoder::HEADER_MINIMUM_LENGTH + 16;
        }

        uint8_t* PacketInDecoder::getEthernetHeader()
        {
            FlowMatchDecoder match(getOxmMatchHeader());
            uint16_t len = match.getFlowMatchLength();
            return getOxmMatchHeader() + len + match.getPadding() + 2;
        }
    }
}
