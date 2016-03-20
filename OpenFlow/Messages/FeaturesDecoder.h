#ifndef ROX_FEATURESREQUEST_H
#define ROX_FEATURESREQUEST_H
#include "HeaderDecoder.h"
#include "Network/Endian.h"
namespace OpenFlow
{
    namespace Messages
    {
        class FeaturesDecoder : public HeaderDecoder
        {
        public:
            enum ofp_capabilities {
                OFPC_FLOW_STATS = 1 << 0,
                OFPC_TABLE_STATS = 1 << 1,
                OFPC_PORT_STATS = 1 << 2,
                OFPC_GROUP_STATS = 1 << 3,
                OFPC_IP_REASM = 1 << 5,
                OFPC_QUEUE_STATS = 1 << 6,
                OFPC_PORT_BLOCKED = 1 << 8
            };

            explicit FeaturesDecoder(uint8_t *p_)
                    : HeaderDecoder(p_)
            {
            }

            FeaturesDecoder(FeaturesDecoder const &decoder) : HeaderDecoder(decoder)
            {
            }

            uint64_t getDatapathId()
            {
                uint64_t *temp = (uint64_t*)(p+HEADER_MINIMUM_LENGTH);
                return htonll(*temp);
            }

            uint32_t getNBuffers()
            {
                uint32_t *temp = (uint32_t*)(p+HEADER_MINIMUM_LENGTH+8);
                return htonl(*temp);
            }

            uint8_t getNTables()
            {
                return p[HEADER_MINIMUM_LENGTH+8+4];
            }

            uint8_t getAuxiliaryId()
            {
                return p[HEADER_MINIMUM_LENGTH+8+4+1];
            }

            uint32_t getCapabilities()
            {
                uint32_t *temp = (uint32_t*)(p+HEADER_MINIMUM_LENGTH+8+8);
                return htonl(*temp);
            }
        };
    }
}
#endif // ROX_FEATURESREQUEST_H