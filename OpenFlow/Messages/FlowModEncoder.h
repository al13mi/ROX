#ifndef ROX_FLOW_MOD_ENCODER_H
#define ROX_FLOW_MOD_ENCODER_H

#include "HeaderEncoder.h"
#include <stdint.h>

namespace OpenFlow
{
    namespace Messages
    {
        class FlowModEncoder : public HeaderEncoder
        {
        public:
            FlowModEncoder(uint8_t *p_);
            FlowModEncoder(FlowModEncoder const &decoder);
            void setCookie(uint64_t v);
            void setCookieMask(uint64_t v);
            void setTableId(uint8_t v);
            void setCommand(uint8_t v);
            void setIdleTimeout(uint16_t v);
            void setHardTimeout(uint16_t v);
            void setPriority(uint16_t v);
            void setBufferId(uint32_t v);
            void setOutPort(uint32_t v);
            void setOutGroup(uint32_t v);
            void setFlags(uint16_t v);
            void setImportance(uint16_t v);
            uint8_t *getMatchFieldWritePtr();
            void padMatchField();
            uint16_t getFlowModMinLength();
        };
    }
}
#endif