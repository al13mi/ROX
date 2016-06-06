#ifndef ROX_OXMTLV_DECODER_H
#define ROX_OXMTLV_DECODER_H

#include <stdint.h>

namespace OpenFlow
{
    namespace Messages
    {
        class OxmTLV
        {
        protected:
            uint8_t *p;
        public:
            explicit OxmTLV(uint8_t *p_);
            OxmTLV(OxmTLV const &decoder);
            uint16_t getOxmClass();
            void setOxmClass(uint16_t v);
            uint8_t getOxmField();
            void setOxmField(uint8_t v);
            uint8_t getOxmLength();
            void setOxmLength(uint8_t v);
            void setOxmValue(uint64_t v, uint32_t len = 4);
        };
    }
}
#endif
