#ifndef ROX_MATCH_INSTRUCTION_ENCODER_H
#define ROX_MATCH_INSTRUCTION_ENCODER_H

#include <stdint.h>

namespace OpenFlow
{
    namespace Messages
    {
        class FlowModInstructionEncoder
        {
        protected:
            uint8_t *p;
        public:
            explicit FlowModInstructionEncoder(uint8_t *p_);

            void setType(uint16_t v);

            void setLength(uint16_t v);
            void setPadding();
            uint16_t getLength();
        };
    }
}
#endif