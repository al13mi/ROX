#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <iostream>
#include <mutex>
#include <thread>
#include "System/LookupTree.h"
#include "Network/ArpTable.h"
#include "OpenFlow/OpenFlowTable.h"
#include <memory>
#include <stack>


enum
{
    LISTEN_PORT = 6653,
    BUFFER_SIZE = 2640,
};

struct Mbuf
{
    uint8_t buf[BUFFER_SIZE];
    ssize_t size;
};

namespace OpenFlow
{

    class Controller
    {
    public:
        Controller();
        void ControllerThread();
        int rxPacket(unsigned char *buf, ssize_t size);
        void connectionHandler();
        void rxHandler();
        uint32_t getMessageLength(uint8_t *buf, ssize_t size);

    private:
        void echoRequestHandler(unsigned char *buf, ssize_t size);
        void featureRequestHandler(unsigned char *buf, ssize_t size);
        void helloHandler(unsigned char * buf, ssize_t size);
        void featuresReplyHandler(unsigned char *buf, ssize_t size);
        void pktInDecoder(unsigned char *buf, ssize_t size);
        void flowRemovedHandler(unsigned char *buf, ssize_t size);

        uint32_t version;
        uint32_t xid;
        Mbuf rxRingBuf[BUFFER_SIZE];
        uint8_t txBuf[BUFFER_SIZE*4];
        uint32_t read = 0;
        uint32_t write = 0;

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
        OpenFlowTable m_table;

        // Locking order is rx then tx.
        std::mutex rxLock;
        std::mutex txLock;
        std::thread T;
    };
}

#endif

