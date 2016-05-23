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


#include "Network/Ethernet.h"
#include "Network/IpAddressV4.h"
#include "Network/MacAddress.h"

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
        /**
        Network::IpAddressV4 route("192.168.1.0");
        Network::IpAddressV4 nextHop("192.168.1.254");
        m_routeTable->insert(route, 24, nextHop);

        Network::IpAddressV4 route2("192.168.2.0");
        Network::IpAddressV4 nextHop2("192.168.2.254");
        m_routeTable->insert(route, 24, nextHop);

        // Create an Arp Entry for the next hop.
        Network::IpAddressV4 address("192.168.1.254");
        uint64_t mac = 0x520000000001;
        m_arpTable->insertArpEntry(mac, address);

        Network::IpAddressV4 address2("192.168.2.254");
        uint64_t mac2 = 0x530000000001;
        m_arpTable->insertArpEntry(mac, address2);
         **/
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

        // TODO: put this into a flowTable abstraction.
        uint8_t newBuf[1500];
        uint8_t *flowModStart = newBuf;
        OpenFlow::Messages::FlowModEncoder flowModEncoder(flowModStart);
        flowModEncoder.setType(OFPT_FLOW_MOD);
        flowModEncoder.setXid(xid++);
        flowModEncoder.setVersion(version);

        flowModEncoder.setCookie(0);
        flowModEncoder.setCookieMask(0);
        flowModEncoder.setTableId(0);
        flowModEncoder.setCommand(OFPFC_ADD);
        flowModEncoder.setIdleTimeout(0);
        flowModEncoder.setHardTimeout(0);
        flowModEncoder.setPriority(1);
        flowModEncoder.setBufferId(OFP_NO_BUFFER);
        flowModEncoder.setOutGroup(OFPG_ANY);
        flowModEncoder.setFlags(0);
        flowModEncoder.setOutPort(1);

        uint8_t* match = flowModEncoder.getMatchFieldWritePtr();
        OpenFlow::Messages::FlowMatchEncoder flowMatchEncoder(match);
        flowMatchEncoder.setFlowMatchType(OFPMT_OXM);
        uint8_t* oxmFields = flowMatchEncoder.getOxmFields();

        OpenFlow::Messages::OxmTLV tlv(oxmFields);
        tlv.setOxmClass(OFPXMC_OPENFLOW_BASIC); // 16
        tlv.setOxmField(0); // 8
        tlv.setOxmValue(1); // 32
        tlv.setOxmLength(4);
        flowMatchEncoder.setFlowMatchLength(12);

        uint8_t *endPtr = oxmFields + 12;
        uint16_t pktLen = endPtr - txBuf;

        uint8_t* flowInstructionEncoderStart = endPtr;
        OpenFlow::Messages::FlowModInstructionEncoder instruction(endPtr);
        instruction.setType(OFPIT_APPLY_ACTIONS);
        instruction.setPadding();

        endPtr += instruction.getLength();

        OpenFlow::Messages::FlowModActionEncoder action(endPtr);
        action.setType(OFPAT_OUTPUT);
        action.setOutputPort(OFPP_CONTROLLER);
        action.setMaxLen(0xffff);
        action.setPadding();
        action.setActionLen(action.getLength());
        endPtr += action.getLength();

        instruction.setLength(endPtr - flowInstructionEncoderStart);
        flowModEncoder.setLength(endPtr - flowModStart);
        txPacket(flowModStart, endPtr - flowModStart);

    }

    void Controller::pktInDecoder(unsigned char *buf, ssize_t size)
    {
        OpenFlow::Messages::PacketInDecoder decoder(buf);
        if(decoder.getReason() == OFPR_TABLE_MISS)
        {
            // Parse out the packet.
            unsigned char *pkt = decoder.getEthernetHeader();
            Network::EthernetHeader *ethernetHeader = (Network::EthernetHeader*)pkt;
            uint16_t etherType =  *((uint16_t*)ethernetHeader->optional.type.ethertype);
            if(htons(Network::IPV4) == etherType)
            {
                Network::EthernetHeader::IpHeaderV4 *iphdr = (Network::EthernetHeader::IpHeaderV4*)ethernetHeader->optional.type.payload;
                Network::MacAddress mac(ethernetHeader->sourceMac);

                /**
                Network::IpAddressV4 source;
                source.data.word = ntohl(iphdr->source);
                Network::IpAddressV4 destination;
                destination.data.word = ntohl(iphdr->destination);
                // Do a route lookup
                //Network::IpAddressV4 nextHop;
                //m_routeTable.getMatchingPrefix(nextHop);

                // TODO: We need an if interface table.

                // Store off the mac address for this to save time.
                m_arpTable->insertArpEntry(mac.data.word, address);
                **/
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
