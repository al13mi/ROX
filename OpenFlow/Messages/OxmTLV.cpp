#include "OxmTLV.h"
#include <arpa/inet.h>

namespace OpenFlow
{
    namespace Messages
    {
        OxmTLV::OxmTLV(uint8_t *p_)
                : p(p_)
        {
        }

        OxmTLV::OxmTLV(OxmTLV const &decoder)
        {
        }

        uint16_t OxmTLV::getOxmClass()
        {
            return ntohs(p[0]);
        }

        void OxmTLV::setOxmClass(uint16_t v)
        {
            *(uint16_t*)p = htons(v);
        }

        uint8_t OxmTLV::getOxmField()
        {
            return p[2];
        }

        void OxmTLV::setOxmField(uint8_t v)
        {
            *(p+2) = v;
        }

        uint8_t OxmTLV::getOxmLength()
        {
            return p[3];
        }

        void OxmTLV::setOxmLength(uint8_t v)
        {
            *(p+3) = v;
        }

        void OxmTLV::setOxmValue(uint32_t v)
        {
            *(uint32_t*)(p+4) = htonl(v);
        }
    }
}
