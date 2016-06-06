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
            return p[2] >> 1;
        }

        void OxmTLV::setOxmField(uint8_t v)
        {
            uint8_t temp = v << 1;
            *(p+2) = temp;
        }

        uint8_t OxmTLV::getOxmLength()
        {
            return p[3];
        }

        void OxmTLV::setOxmLength(uint8_t v)
        {
            *(p+3) = v;
        }

        void OxmTLV::setOxmValue(uint64_t v, uint32_t len)
        {
            switch(len)
            {
                case 1:
                    *(uint8_t*)(p+4) = v;
                    break;
                case 2:
                    *(uint16_t*)(p+4) = htons(v);
                    break;
                case 4:
                    *(uint32_t*)(p+4) = htonl(v);
                    break;
                case 6:
                    uint8_t* temp = (uint8_t*)&v;
                    for(uint32_t n = 0; n<6; n++)
                    {
                        *(uint8_t*)(p+ 4 + n) = temp[5 - n];
                    }
                    break;

            }

        }
    }
}
