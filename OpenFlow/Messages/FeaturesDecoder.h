#ifndef ROX_FEATURESREQUEST_H
#define ROX_FEATURESREQUEST_H

#include "HeaderDecoder.h"

namespace OpenFlow
{
    namespace Messages
    {
        class FeaturesDecoder : public HeaderDecoder
        {
        public:
            explicit FeaturesDecoder(uint8_t *p_);
            FeaturesDecoder(FeaturesDecoder const &decoder);
            uint64_t getDatapathId();
            uint32_t getNBuffers();
            uint8_t getNTables();
            uint8_t getAuxiliaryId();
            uint32_t getCapabilities();
        };
    }
}
#endif // ROX_FEATURESREQUEST_H