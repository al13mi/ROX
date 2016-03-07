#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <iostream>
#include <mutex>
#include <thread>

enum
{
    LISTEN_PORT = 6653,
    BUFFER_SIZE = 2000,
};


namespace OpenFlow
{    
    class Controller
    {
    public:
        Controller();
        int rxPacket(unsigned char *buf, ssize_t size);
    private:
        void echoHandler(unsigned char *buf, ssize_t size);
        void featureRequestHandler(unsigned char *buf, ssize_t size);
        void helloHandler(unsigned char * buf, ssize_t size);

        void featureRequestFactory(unsigned char *buf);
        void helloFactory(unsigned char *buf);
        void echoReplyFactory(unsigned char *buf);
        

        uint32_t version;
        uint32_t xid;
        
    };
}

#endif

