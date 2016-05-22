#ifndef ROX_MATCH_INSTRUCTION_ENCODER_H
#define ROX_MATCH_INSTRUCTION_ENCODER_H
#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FlowModInstructionEncoder
        {
        protected:
            uint8_t *p;
        public:
            enum ofp_instruction_type {
                OFPIT_GOTO_TABLE = 1,
                OFPIT_WRITE_METADATA = 2,
                OFPIT_WRITE_ACTIONS = 3,
                OFPIT_APPLY_ACTIONS = 4,
                OFPIT_CLEAR_ACTIONS = 5,
                OFPIT_METER = 6,
                OFPIT_EXPERIMENTER = 0xFFFF
            };


            explicit FlowModInstructionEncoder(uint8_t *p_)
                    : p(p_)
            {
            }

            void setType(uint16_t v)
            {
                *(uint16_t*)p = htons(v);
            }

            void setLength(uint16_t v)
            {
                *(uint16_t*)(p+2) = htons(v);
            }

            void setPadding()
            {
                *(uint32_t*)(p+4) = 0;
            }

            uint16_t getLength()
            {
                return 8;
            }
        };
    }
}
#endif