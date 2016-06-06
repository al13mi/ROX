#include "Controller.h"

#include <iostream>
#include <netinet/in.h>
#include <bitset>
#include <string.h>

#include "OpenFlow/EnumTypes.h"
#include "OpenFlow/Messages/HeaderEncoder.h"
#include "OpenFlow/Messages/HelloDecoder.h"
#include "OpenFlow/Messages/FeaturesDecoder.h"
#include "OpenFlow/Messages/FlowMatchEncoder.h"
#include "OpenFlow/Messages/OxmTLV.h"
#include "OpenFlow/Messages/PacketInDecoder.h"
#include "OpenFlow/Messages/FlowModEncoder.h"
#include "OpenFlow/Messages/FlowModInstructionEncoder.h"
#include "OpenFlow/Messages/FlowModActionEncoder.h"

#include "OpenFlow/OpenFlowTable.h"

#include "Network/Ethernet.h"
#include "Network/IpAddressV4.h"
#include "Network/MacAddress.h"

#include <memory>


#define OFP_NO_BUFFER 0xffffffff

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
        memset(txBuf, 0, BUFFER_SIZE);

        m_routeTable = std::unique_ptr<System::LookupTree>(new System::LookupTree());
        m_arpTable = std::unique_ptr<Network::ArpTable>(new Network::ArpTable());

        // TODO: create an IfInterfaceTable to go from route to egress if number, and MacAddress

        // 192.168.1.*
        Network::IpAddressV4 route(3232235776);
        Network::IpAddressV4 nextHop(3232236030);
        m_routeTable->insert(route, 24, nextHop);

        Network::IpAddressV4 route2(3232236032);
        Network::IpAddressV4 nextHop2(3232236286);
        m_routeTable->insert(route2, 24, nextHop2);

        // Create an Arp Entry for the next hop.
        Network::IpAddressV4 address(3232236030);
        uint64_t mac = 0x520000000001;
        m_arpTable->insertArpEntry(mac, address);

        Network::IpAddressV4 address2(3232236286);
        uint64_t mac2 = 0x530000000001;
        m_arpTable->insertArpEntry(mac2, address2);
    }

    void Controller::connectionHandler()
    {
        uint8_t buf[1500];
        OpenFlow::Messages::HeaderEncoder encoder(buf);
        encoder.setLength(OpenFlow::Messages::HeaderDecoder::HEADER_MINIMUM_LENGTH);
        encoder.setType(OFPT_HELLO);
        encoder.setXid(xid++);
        encoder.setVersion(version);
        txPacket(encoder.getReadPtr(), encoder.getLength());
    }


    int Controller::rxPacket(uint8_t *buf, ssize_t size)
    {
        print_buf("rx: ", buf, size);

        OpenFlow::Messages::HeaderDecoder decoder(buf);
        uint16_t type = decoder.getType();

    #ifdef DEBUG
        cout << "Rxpacket: recv: " << size << " bytes\n";
        cout << "Version:\t" << decoder.getVersion() << endl;
        cout << "Type:\t " << decoder.getType() << endl;
        cout << "Length:\t " << decoder.getLength() << endl;
        cout << "Xid:\t " << decoder.getXid() << endl;
    #endif    

    
        switch(type)
        {
            case OFPT_HELLO:
            {
                helloHandler(buf, size);
                break;
            }
            case OFPT_ECHO_REQUEST:
            {
                echoRequestHandler(buf, size);
                break;
            }
            case OFPT_FEATURES_REPLY:
            {
                featuresReplyHandler(buf, size);
                break;
            }
            case OFPT_PACKET_IN:
            {
                pktInDecoder(buf, size);
                break;
            }

        }
        return 0;
    }
    
    void Controller::echoRequestHandler(unsigned char *buf, ssize_t size)
    {
        OpenFlow::Messages::HeaderEncoder encoder(buf);
        encoder.setVersion(version);
        encoder.setType(OFPT_ECHO_REPLY);
        encoder.setLength(OpenFlow::Messages::HeaderDecoder::HEADER_MINIMUM_LENGTH);
        txPacket(encoder.getReadPtr(), encoder.getLength());
    }

    void Controller::helloHandler(unsigned char *buf, ssize_t size)
    {
        OpenFlow::Messages::HelloDecoder decoder(buf);
        // Check to see if their latest version is our latest version
        uint32_t latestVersion = decoder.getVersion();

        uint16_t msgLength = 0;
        if(version == latestVersion)
        {
            OpenFlow::Messages::HeaderEncoder encoder(txBuf);
            encoder.setType(OFPT_FEATURES_REQUEST);
            encoder.setXid(xid++);
            encoder.setVersion(version);
            encoder.setLength(OpenFlow::Messages::HeaderEncoder::HEADER_MINIMUM_LENGTH);
            msgLength += encoder.getLength();
            txPacket(txBuf, msgLength);
        }
        else
        {
            // Send an error
        }
    }

    void Controller::featuresReplyHandler(unsigned char *buf, ssize_t size)
    {
        OpenFlow::Messages::FeaturesDecoder decoder(buf);
        switchFeatures.capabilities = decoder.getCapabilities();
        switchFeatures.datapathId = decoder.getDatapathId();
        switchFeatures.nBuffers = decoder.getNBuffers();
        switchFeatures.nTables = decoder.getNTables();
        switchFeatures.auxiliaryId = decoder.getAuxiliaryId();


        uint16_t len = m_table.buildExceptionPath(txBuf, 1);


        OpenFlow::Messages::HeaderEncoder encoder(txBuf);
        encoder.setType(OFPT_FLOW_MOD);
        encoder.setXid(xid++);
        encoder.setVersion(version);

        // uint32_t pad = 8 - len%8;

        encoder.setLength(len);
        txPacket(txBuf, len);

        len = m_table.buildExceptionPath(txBuf, 2);

        encoder.setLength(len);

        txPacket(txBuf, len);

    }

    void Controller::pktInDecoder(unsigned char *buf, ssize_t size)
    {
        OpenFlow::Messages::PacketInDecoder decoder(buf);

        OpenFlow::Messages::HeaderEncoder encoder(txBuf);
        encoder.setType(OFPT_FLOW_MOD);
        encoder.setXid(xid++);
        encoder.setVersion(version);

        if(decoder.getReason() == OFPR_APPLY_ACTION)
        {
            // Parse out the packet.
            unsigned char *pkt = decoder.getEthernetHeader();
            Network::EthernetHeader *ethernetHeader = (Network::EthernetHeader*)pkt;
            uint16_t etherType =  *((uint16_t*)ethernetHeader->optional.type.ethertype);
            if(htons(Network::IPV4) == etherType)
            {
                Network::IpHeaderV4 *iphdr = (Network::IpHeaderV4*)ethernetHeader->optional.type.payload;

                uint32_t *temp = (uint32_t*)(iphdr->destination);
                Network::IpAddressV4 addr(ntohl(*temp));

                // TODO: Check to see if we have a flow hit.
                Network::FlowIndexV4 index(iphdr);

                // There wasn't a flow hit so lets do a route lookup.
                Network::IpAddressV4 nextHop;
                m_routeTable->getMatchingPrefix(addr, nextHop);

                uint64_t mac;
                m_arpTable->findArpEntry(mac, nextHop);

                // Lookup the source Interface table (which means I need an interface table).
                uint16_t len = m_table.addFlowEntryFromIndexV4(txBuf, &index, 2, 0x5200000000AA, mac);
                encoder.setLength(len);

                txPacket(txBuf, len);
            }
            else if(htons(Network::ARP) == etherType)
            {
                // TODO: implement arp.
            }

        }
    }

    Controller::SwitchFeatures::SwitchFeatures()
        : datapathId(0)
        , nBuffers(0)
        , capabilities(0)
        , nTables(0)
        , auxiliaryId(0)
    {
    }

}
