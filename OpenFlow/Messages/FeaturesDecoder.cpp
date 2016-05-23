#include "FeaturesDecoder.h"
#include <arpa/inet.h>

namespace OpenFlow
{
    namespace Messages {

        FeaturesDecoder::FeaturesDecoder(uint8_t *p_) : HeaderDecoder(p_) {
        }

        FeaturesDecoder::FeaturesDecoder(FeaturesDecoder const &decoder) : HeaderDecoder(decoder) {
        }

        uint64_t FeaturesDecoder::getDatapathId() {
            uint64_t *temp = (uint64_t * )(p + HEADER_MINIMUM_LENGTH);
            return htobe64(*temp);
        }

        uint32_t FeaturesDecoder::getNBuffers() {
            uint32_t *temp = (uint32_t * )(p + HEADER_MINIMUM_LENGTH + 8);
            return htonl(*temp);
        }

        uint8_t FeaturesDecoder::getNTables() {
            return p[HEADER_MINIMUM_LENGTH + 8 + 4];
        }

        uint8_t FeaturesDecoder::getAuxiliaryId() {
            return p[HEADER_MINIMUM_LENGTH + 8 + 4 + 1];
        }

        uint32_t FeaturesDecoder::getCapabilities() {
            uint32_t *temp = (uint32_t * )(p + HEADER_MINIMUM_LENGTH + 8 + 8);
            return htonl(*temp);
        }
    }
}
