#include "Controller.h"

#include <iostream>
#include <netinet/in.h>
#include "OpenFlow.h"
using namespace std;

extern void txPacket(unsigned char *buf, ssize_t size);

static void print_buf(const char *title, const unsigned char *buf, size_t buf_len)
{
#ifdef DEBUG
    size_t i = 0;
    fprintf(stdout, "\n%s\n", title);
    for(i = 0; i < buf_len; ++i)
    fprintf(stdout, "%02X%s", buf[i],
    ( i + 1 ) % 16 == 0 ? "\r\n" : " " );
    printf("\n");
#endif
}

namespace OpenFlow
{
    Controller::Controller()
    {
        version = 0x5;
        xid = 1;    
    }
    
    int Controller::rxPacket(unsigned char *buf, ssize_t size)
    {
        ofp_header *header = (ofp_header*)buf;
        print_buf("rx: ", buf, size);
    #ifdef DEBUG
        cout << "Rxpacket: recv: " << size << " bytes\n";
        cout << "Version:\t" << (uint16_t)header->version << endl;
        cout << "Type:\t " << (uint16_t)header->type << endl;
        cout << "Length:\t " << ntohs(header->length) << endl;
        cout << "Xid:\t " << ntohl(header->xid) << endl;
    #endif    
        
        uint16_t type = header->type;
    
        switch(type)
        {
            case HELLO:
            {
                helloHandler(buf, size);
                break;
            }
            case ECHO_REQUEST:
            {
                echoHandler(buf, size);
                break;
            }
        }
        return 0;
    }
    
    void Controller::echoHandler(unsigned char *buf, ssize_t size)
    {
        echoReplyFactory(buf);
        txPacket(buf, sizeof(ofp_header));
    }
    
    void Controller::echoReplyFactory(unsigned char *outbuf)
    {
        ofp_header *header = (ofp_header*)outbuf;
        
        header->version = version;
        header->type = ECHO_REPLY;
        header->length = htons(sizeof(ofp_header));
    }
    
    void Controller::featureRequestFactory(unsigned char* outbuf)
    {
        ofp_header *hdr = (ofp_header*)outbuf;
        hdr->version = version;
        hdr->length = htons(sizeof(ofp_header));
        hdr->type = FEATURES_REQUEST;
        hdr->xid = htonl(xid++);
    }
    
    void Controller::helloFactory(unsigned char* outbuf)
    {
        ofp_hello *hi = (ofp_hello*)outbuf;
        
        hi->header.version = version;
        hi->header.type = HELLO;
        hi->header.length = htons(sizeof(ofp_header));
        hi->header.xid = htonl(xid++);
    }
    
    void Controller::helloHandler(unsigned char *buf, ssize_t size)
    {
        // For now lets just assume this worked
        // TODO: need error handling here.  We also need to check the bitset
        helloFactory(buf);
        txPacket(buf, sizeof(ofp_header));
        featureRequestFactory(buf);
        txPacket(buf, sizeof(ofp_header));
    }

}
