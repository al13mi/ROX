//
// Created by marnold on 3/19/16.
//

#ifndef ROX_HEADERDECODER_H
#define ROX_HEADERDECODER_H

#include "Network/Endian.h"

namespace OpenFlow
{
    namespace Messages
    {
        class HeaderDecoder
        {
        protected:
            uint8_t *p;
        public:
            enum
            {
                OFPT_HELLO = 0,
                OFPT_ERROR = 1,
                OFPT_ECHO_REQUEST = 2,
                OFPT_ECHO_REPLY = 3,
                OFPT_EXPERIMENTER = 4,
                OFPT_FEATURES_REQUEST = 5,
                OFPT_FEATURES_REPLY = 6,
                OFPT_GET_CONFIG_REQUEST = 7,
                OFPT_GET_CONFIG_REPLY = 8,
                OFPT_SET_CONFIG = 9,
                OFPT_PACKET_IN = 10,
                OFPT_FLOW_REMOVED = 11,
                OFPT_PORT_STATUS = 12,
                OFPT_PACKET_OUT = 13,
                OFPT_FLOW_MOD = 14,
                OFPT_GROUP_MOD = 15,
                OFPT_PORT_MOD = 16,
                OFPT_TABLE_MOD = 17,
                OFPT_MULTIPART_REQUEST = 18,
                OFPT_MULTIPART_REPLY = 19,
                OFPT_BARRIER_REQUEST = 20,
                OFPT_BARRIER_REPLY = 21,
                OFPT_ROLE_REQUEST = 24,
                OFPT_ROLE_REPLY = 25,
                OFPT_GET_ASYNC_REQUEST = 26,
                OFPT_GET_ASYNC_REPLY = 27,
                OFPT_SET_ASYNC = 28,
                OFPT_METER_MOD = 29,
                OFPT_ROLE_STATUS = 30,
                OFPT_TABLE_STATUS = 31,
                OFPT_REQUESTFORWARD = 32,
                OFPT_BUNDLE_CONTROL = 33,
                OFPT_BUNDLE_ADD_MESSAGE = 34,
            };

            enum { HEADER_MINIMUM_LENGTH = 8};

            explicit HeaderDecoder(uint8_t *p_)
                    : p(p_) {
            }

            HeaderDecoder(HeaderDecoder const &decoder) : p(decoder.p)
            {
            }

            uint8_t getVersion() const
            {
                return p[0];
            }

            uint8_t getType() const
            {
                return p[1];
            }

            uint16_t getLength() const
            {
                uint16_t *temp = (uint16_t*)(p+2);
                return ntohs(*temp);
            }

            uint32_t getXid() const
            {
                uint32_t *temp = (uint32_t*)(p+4);
                return ntohl(*temp);
            }

            uint8_t *getReadPtr()
            {
                return p;
            }


            static bool isMinimumLengthOk(const uint8_t *readPtr, const uint8_t *writePtr)
            {
                return (writePtr - readPtr) >= HEADER_MINIMUM_LENGTH;
            }
        };
    }
}

#endif //ROX_HEADERDECODER_H
