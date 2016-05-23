#ifndef ROX_MATCH_ACTION_ENCODER_H
#define ROX_MATCH_ACTION_ENCODER_H

#include <stdint.h>

namespace OpenFlow
{
    namespace Messages
    {
        class FlowModActionEncoder
        {
        private:
            uint8_t *p;
        public:
            FlowModActionEncoder(uint8_t *p_);
            void setType(uint16_t v);
            void setActionLen(uint16_t v);
            void setOutputPort(uint32_t v);
            void setMaxLen(uint16_t v);
            void setPadding();
            uint16_t getLength();
        };
    }
}
#endif