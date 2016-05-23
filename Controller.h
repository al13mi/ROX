#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <iostream>
#include <mutex>
#include <thread>
#include "System/LookupTree.h"
#include "Network/ArpTable.h"
#include <memory>
#include <stack>


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
        void connectionHandler();
    private:
        void echoRequestHandler(unsigned char *buf, ssize_t size);
        void featureRequestHandler(unsigned char *buf, ssize_t size);
        void helloHandler(unsigned char * buf, ssize_t size);
        void featuresReplyHandler(unsigned char *buf, ssize_t size);
        void pktInDecoder(unsigned char *buf, ssize_t size);

        uint32_t version;
        uint32_t xid;
        uint8_t txBuf[BUFFER_SIZE];


        struct SwitchFeatures
        {
            uint64_t datapathId;
            uint32_t nBuffers;
            uint32_t capabilities;
            uint16_t nTables;
            uint16_t auxiliaryId;

            SwitchFeatures();
        };
        SwitchFeatures switchFeatures;

        std::unique_ptr<System::LookupTree> m_routeTable;
        std::unique_ptr<Network::ArpTable> m_arpTable;
    };
}

#endif

