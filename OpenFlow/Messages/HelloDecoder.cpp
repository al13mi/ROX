#include <stdint.h>
#include <arpa/inet.h>
#include "HelloDecoder.h"
namespace OpenFlow
{
    namespace Messages
    {
        HelloDecoder::HelloDecoder(uint8_t *p_)
                : HeaderDecoder(p_)
        {
        }

        HelloDecoder::HelloDecoder(HelloDecoder const &decoder) : HeaderDecoder(decoder)
        {
        }

        uint16_t HelloDecoder::getNumHelloElements()
        {
            return (HelloDecoder::getLength() - HEADER_MINIMUM_LENGTH)/ HELLO_ELEMENT_SIZE;
        }

        uint8_t *HelloDecoder::getHelloElement(int num)
        {
            return &p[HEADER_MINIMUM_LENGTH + num];
        }
    }
}

