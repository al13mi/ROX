#include "OpenFlowTable.h"

#include "EnumTypes.h"
#include "Messages/OxmTLV.h"
#include "Messages/HeaderEncoder.h"
#include "Messages/HelloDecoder.h"
#include "Messages/FeaturesDecoder.h"
#include "Messages/FlowMatchEncoder.h"
#include "Messages/PacketInDecoder.h"
#include "Messages/FlowModEncoder.h"
#include "Messages/FlowModInstructionEncoder.h"
#include "Messages/FlowModActionEncoder.h"
#include "System/crc32.h"
#include <chrono>

extern void txPacket(unsigned char *buf, ssize_t size);

#define OFP_NO_BUFFER 0xffffffff

namespace OpenFlow {

    uint16_t OpenFlowSetL2Src::buildPacketData(uint8_t *startPtr)
    {
        uint16_t total = 0;

        OpenFlow::Messages::FlowModActionEncoder action(startPtr);
        action.setType(OFPAT_SET_FIELD);

        OpenFlow::Messages::OxmTLV srcTlv(startPtr + 4);
        srcTlv.setOxmClass(OFPXMC_OPENFLOW_BASIC); // 16
        srcTlv.setOxmField(OFPXMT_OFB_ETH_SRC);
        srcTlv.setOxmLength(6);
        srcTlv.setOxmValue(m_src, 6);
        total += 4 + action.getLength();

        total += 8 - total % 8;

        action.setActionLen(total);
        return total;
    }

    OpenFlowSetL2Src::OpenFlowSetL2Src(uint64_t src)
            : m_src(src)
    {
    }

    uint16_t OpenFlowSetL2Dst::buildPacketData(uint8_t *startPtr)
    {

        uint16_t total = 0;

        OpenFlow::Messages::FlowModActionEncoder action(startPtr);
        action.setType(OFPAT_SET_FIELD);

        OpenFlow::Messages::OxmTLV dstTlv(startPtr + 4);
        dstTlv.setOxmClass(OFPXMC_OPENFLOW_BASIC); // 16
        dstTlv.setOxmField(OFPXMT_OFB_ETH_DST);
        dstTlv.setOxmLength(6);
        dstTlv.setOxmValue(m_dst, 6);
        total += 4 + action.getLength();

        total += 8 - total % 8;

        action.setActionLen(total);
        return total;
    }

    OpenFlowSetL2Dst::OpenFlowSetL2Dst(uint64_t dst)
            : m_dst(dst)
    {
    }


    uint16_t OpenFlowActionOutput::buildPacketData(uint8_t *startPtr)
    {
        uint8_t *sPtr = startPtr;
        OpenFlow::Messages::FlowModActionEncoder action(sPtr);
        action.setType(OFPAT_OUTPUT);
        action.setOutputPort(m_port);
        action.setMaxLen(0xffff);
        action.setPadding();
        action.setActionLen(action.getLength());
        return action.getLength();
    }

    OpenFlowActionOutput::OpenFlowActionOutput(uint32_t port, uint32_t maxLength)
        : m_port(port)
        , m_maxLength(maxLength)
    {
    }

    uint16_t OpenFlowInstruction::buildPacketData(uint8_t *startPtr)
    {
        uint8_t *sPtr = startPtr;
        OpenFlow::Messages::FlowModInstructionEncoder instruction(sPtr);
        instruction.setType(OFPIT_APPLY_ACTIONS);
        instruction.setPadding();
        uint16_t len = instruction.getLength();
        sPtr += len;
        std::list<std::unique_ptr<OpenFlowAction>>::iterator it;
        for (it = m_action.begin(); it != m_action.end(); ++it)
        {
            len += (*it)->buildPacketData(sPtr);
            sPtr = startPtr + len;
        }

        instruction.setLength(len);

        return len;
    }

    void OpenFlowInstruction::insertAction(std::unique_ptr<OpenFlowAction> action)
    {
        m_action.push_front(std::move(action));
    }

    OpenFlowInstruction::OpenFlowInstruction(uint16_t type)
        : m_type(type)
    {

    }

    uint16_t OpenFlowMatchFields::buildPacketData(uint8_t *start)
    {
        uint8_t* sPtr = start;
        OpenFlow::Messages::OxmTLV tlv(sPtr);
        tlv.setOxmClass(OFPXMC_OPENFLOW_BASIC); // 16
        tlv.setOxmField(m_field);

        uint32_t total = 3;

        switch(m_field)
        {
            // 8 bits
            case OFPXMT_OFB_IP_DSCP:
            case OFPXMT_OFB_IP_PROTO:
                tlv.setOxmLength(1);
                tlv.setOxmValue(m_value, 1);
                total += 2;
                break;
            // 16 bits
            case OFPXMT_OFB_TCP_SRC:
            case OFPXMT_OFB_TCP_DST:
            case OFPXMT_OFB_UDP_SRC:
            case OFPXMT_OFB_UDP_DST:
            case OFPXMT_OFB_ETH_TYPE:
                tlv.setOxmLength(2);
                tlv.setOxmValue(m_value, 2);
                total += 3;
                break;

            // 32 bits
            case OFPXMT_OFB_IN_PORT:
            case OFPXMT_OFB_IPV4_SRC:
            case OFPXMT_OFB_IPV4_DST:
                tlv.setOxmLength(4);
                tlv.setOxmValue(m_value, 4);
                total += 5;
                break;

            // 48 bits
            case OFPXMT_OFB_ETH_DST:
            case OFPXMT_OFB_ETH_SRC:
                tlv.setOxmLength(6);
                tlv.setOxmValue(m_value, 6);
                total += 7;
                break;

        }

        return total;
    }

    OpenFlowMatchFields::OpenFlowMatchFields(uint16_t oxmClass, uint8_t field, uint64_t value)
        : m_oxmClass(oxmClass)
        , m_field(field)
        , m_value(value)
    {
    }

    OpenFlowMatch::OpenFlowMatch()
    {
    }

    uint16_t OpenFlowMatch::buildPacketData(uint8_t *start)
    {
        uint8_t *sPtr = start;
        OpenFlow::Messages::FlowMatchEncoder flowMatchEncoder(start);
        flowMatchEncoder.setFlowMatchType(OFPMT_OXM);
        sPtr = flowMatchEncoder.getOxmFields();
        uint16_t len = sPtr - start;

        std::list<std::unique_ptr<OpenFlowMatchFields>>::iterator it;
        for (it = m_fields.begin(); it != m_fields.end(); ++it)
        {
            len += (*it)->buildPacketData(sPtr);
            sPtr = start + len;
        }


        flowMatchEncoder.setFlowMatchLength(len);

        // pad to a multiple of 8 bytes.

        uint32_t pad = 8 - len%8;
        len += pad;
        sPtr += pad;


        std::list<std::unique_ptr<OpenFlowInstruction>>::iterator instructionIt;
        for (instructionIt = m_instruction.begin(); instructionIt != m_instruction.end(); ++instructionIt)
        {
            len += (*instructionIt)->buildPacketData(sPtr);
            sPtr = start + len;
        }
        return len;
    }

    void OpenFlowMatch::insertField(std::unique_ptr<OpenFlowMatchFields> field)
    {
        m_fields.push_front(std::move(field));
    }

    void OpenFlowMatch::insertInstruction(std::unique_ptr<OpenFlowInstruction> instruction)
    {
        m_instruction.push_front(std::move(instruction));
    }


    OpenFlowTableEntry::OpenFlowTableEntry(uint64_t cookie, uint64_t cookieMask, uint8_t tableId)
        : m_cookie(cookie)
        , m_cookieMask(cookieMask)
        , m_tableId(tableId)
        , m_command(0)
        , m_idleTimeout(0)
        , m_hardTimeout(0)
        , m_priority(0)
        , m_bufferId(0)
        , m_outPort(0)
        , m_outGroup(0)
        , m_flags(0)
        , m_importance(0)
    {
    }

    void OpenFlowTableEntry::setIdleTimeout(uint16_t idleTimeout) {
        m_idleTimeout = idleTimeout;
    }

    void OpenFlowTableEntry::setHardTimeout(uint16_t hardTimeout) {
        m_hardTimeout = hardTimeout;
    }


    void OpenFlowTableEntry::setPriority(uint16_t priority) {
        m_priority = priority;
    }
    
    void OpenFlowTableEntry::setBufferId(uint32_t bufferId) {
        m_bufferId = bufferId;
    }
    
    void OpenFlowTableEntry::setOutPort(uint32_t outPort) {
        m_outPort = outPort;
    }

    void OpenFlowTableEntry::setOutGroup(uint32_t outGroup) {
        m_outGroup = outGroup;
    }
        
    void OpenFlowTableEntry::setFlags(uint16_t flags) {
        m_flags = flags;
    }
        
    void OpenFlowTableEntry::setImportance(uint16_t importance) {
        m_importance = importance;
    }


    uint16_t OpenFlowTableEntry::buildPacketData(uint8_t *startPtr) {
        uint8_t *sPtr = startPtr;

        OpenFlow::Messages::FlowModEncoder encoder(sPtr);
        encoder.setType(OFPT_FLOW_MOD);
        encoder.setXid(1);

        encoder.setCookie(m_cookie);
        encoder.setCookieMask(m_cookieMask);
        encoder.setTableId(m_tableId);
        encoder.setCommand(OFPFC_ADD);
        encoder.setIdleTimeout(m_idleTimeout);
        encoder.setHardTimeout(m_hardTimeout);
        encoder.setPriority(m_priority);
        encoder.setBufferId(m_bufferId);
        encoder.setOutGroup(m_outGroup);
        encoder.setFlags(m_flags);
        encoder.setOutPort(m_outPort);
        encoder.setImportance(m_importance);

        std::list<std::unique_ptr<OpenFlowMatch>>::iterator it;
        uint8_t* endPtr = encoder.getMatchFieldWritePtr();

        uint16_t len = endPtr - startPtr;
        sPtr = startPtr + len;
        for (it = m_match.begin(); it != m_match.end(); ++it)
        {
            len += (*it)->buildPacketData(sPtr);
            sPtr += len;
        }

        encoder.setLength(len);
        return len;
    }

    void OpenFlowTableEntry::insertMatchCriteria(std::unique_ptr<OpenFlowMatch> match)
    {
        m_match.push_front(std::move(match));
    }

    OpenFlowTable::OpenFlowTable()
    {

    }
    OpenFlowTable::~OpenFlowTable(){}

    uint16_t OpenFlowTable::buildExceptionPath(uint8_t *txBuf, uint16_t port) {
        std::unique_ptr<OpenFlow::OpenFlowTableEntry> entry =
                std::unique_ptr<OpenFlow::OpenFlowTableEntry>(new OpenFlow::OpenFlowTableEntry(0,0, 0));
        entry->setIdleTimeout(0);
        entry->setHardTimeout(0);
        entry->setPriority(1);
        entry->setBufferId(OFP_NO_BUFFER);
        entry->setOutGroup(OFPG_ANY);
        entry->setFlags(0);
        entry->setOutPort(1);

        std::unique_ptr<OpenFlow::OpenFlowActionOutput> outputAction
                = std::unique_ptr<OpenFlow::OpenFlowActionOutput>(new OpenFlow::OpenFlowActionOutput(OFPP_CONTROLLER));


        std::unique_ptr<OpenFlow::OpenFlowInstruction> instruction
                = std::unique_ptr<OpenFlow::OpenFlowInstruction>(new OpenFlow::OpenFlowInstruction(OFPIT_APPLY_ACTIONS));


        std::unique_ptr<OpenFlow::OpenFlowMatchFields> fields
                = std::unique_ptr<OpenFlow::OpenFlowMatchFields>(new  OpenFlow::OpenFlowMatchFields(OFPXMC_OPENFLOW_BASIC, 0, port));


        std::unique_ptr<OpenFlow::OpenFlowMatch> match
                = std::unique_ptr<OpenFlow::OpenFlowMatch>(new OpenFlow::OpenFlowMatch());

        instruction->insertAction(std::move(outputAction));
        match->insertInstruction(std::move(instruction));
        match->insertField(std::move(fields));
        entry->insertMatchCriteria(std::move(match));
        uint16_t len = entry->buildPacketData(txBuf);

        return len;
    }

    void OpenFlowTable::removeFlowEntryByCRC(uint32_t crc)
    {
        auto search = m_flowTable.find(crc);
        if(search != m_flowTable.end())
        {
            m_flowTable.erase(search);
        }
    }

    uint16_t OpenFlowTable::addFlowEntryFromIndexV4(uint8_t *txBuf, Network::FlowIndexV4 *index, uint16_t port, uint64_t srcMac, uint64_t dstMac)
    {
        uint16_t len  = 0;

        uint32_t crc = rte_hash_crc(index->contents.byteKey, index->BYTE_KEY_SIZE, 0);

        // Add this Index to the cache
        addFlowStatsToCache(crc, index);

        // Check to see if we already have an entry for this.
        std::unique_ptr<OpenFlow::OpenFlowTableEntry> entry;
                std::unique_ptr<OpenFlow::OpenFlowTableEntry>(new OpenFlow::OpenFlowTableEntry(crc, 0xFFFFFFFFFFFFFFFF, 0));

        auto search = m_flowTable.find(crc);
        if(search == m_flowTable.end())
        {
            // std::cout << "Adding Flow Id: " << crc << "\n";

            entry = std::unique_ptr<OpenFlow::OpenFlowTableEntry>(
                    new OpenFlow::OpenFlowTableEntry(crc, 0xFFFFFFFFFFFFFFFF, 0));

            uint8_t protocol = index->contents.ip.protocol;
            uint8_t dscp = index->contents.ip.dscp;
            uint32_t srcIp = htonl(index->contents.ip.srcIp);
            uint32_t dstIp = htonl(index->contents.ip.destIp);


            // Build the match criteria
            std::unique_ptr <OpenFlow::OpenFlowMatch> match
                    = std::unique_ptr<OpenFlow::OpenFlowMatch>(new OpenFlow::OpenFlowMatch());

            switch (protocol) {
                case IP_PROTO_TCP:
                case IP_PROTO_UDP:

                    uint32_t srcPort = index->contents.ip.protocolSpecific.tcpUdp.srcPort;
                    uint32_t dstPort = index->contents.ip.protocolSpecific.tcpUdp.destPort;

                    uint8_t srcField = 0;
                    uint8_t dstField = 0;

                    if (protocol == IP_PROTO_TCP) {
                        srcField = OFPXMT_OFB_TCP_SRC;
                        dstField = OFPXMT_OFB_TCP_DST;
                    }
                    else if (protocol == IP_PROTO_UDP) {
                        srcField = OFPXMT_OFB_UDP_SRC;
                        dstField = OFPXMT_OFB_UDP_DST;
                    }

                    std::unique_ptr <OpenFlow::OpenFlowMatchFields> src
                            = std::unique_ptr<OpenFlow::OpenFlowMatchFields>(
                                    new OpenFlow::OpenFlowMatchFields(OFPXMC_OPENFLOW_BASIC, srcField, srcPort));
                    //match->insertField(std::move(src));

                    std::unique_ptr <OpenFlow::OpenFlowMatchFields> dst
                            = std::unique_ptr<OpenFlow::OpenFlowMatchFields>(
                                    new OpenFlow::OpenFlowMatchFields(OFPXMC_OPENFLOW_BASIC, dstField, dstPort));
                    match->insertField(std::move(dst));
                    break;
            }


            std::unique_ptr <OpenFlow::OpenFlowMatchFields> ipProtocolField
                    = std::unique_ptr<OpenFlow::OpenFlowMatchFields>(
                            new OpenFlow::OpenFlowMatchFields(OFPXMC_OPENFLOW_BASIC, OFPXMT_OFB_IP_PROTO, protocol));
            match->insertField(std::move(ipProtocolField));

            std::unique_ptr <OpenFlow::OpenFlowMatchFields> dstIpField
                    = std::unique_ptr<OpenFlow::OpenFlowMatchFields>(
                            new OpenFlow::OpenFlowMatchFields(OFPXMC_OPENFLOW_BASIC, OFPXMT_OFB_IPV4_DST, dstIp));
            match->insertField(std::move(dstIpField));

            std::unique_ptr <OpenFlow::OpenFlowMatchFields> srcIpField
                    = std::unique_ptr<OpenFlow::OpenFlowMatchFields>(
                            new OpenFlow::OpenFlowMatchFields(OFPXMC_OPENFLOW_BASIC, OFPXMT_OFB_IPV4_SRC, srcIp));
            match->insertField(std::move(srcIpField));

            std::unique_ptr <OpenFlow::OpenFlowMatchFields> dscpField
                    = std::unique_ptr<OpenFlow::OpenFlowMatchFields>(
                            new OpenFlow::OpenFlowMatchFields(OFPXMC_OPENFLOW_BASIC, OFPXMT_OFB_IP_DSCP, dscp));
            match->insertField(std::move(dscpField));

            std::unique_ptr <OpenFlow::OpenFlowMatchFields> ethTypeField
                    = std::unique_ptr<OpenFlow::OpenFlowMatchFields>(
                            new OpenFlow::OpenFlowMatchFields(OFPXMC_OPENFLOW_BASIC, OFPXMT_OFB_ETH_TYPE, 0x800));
            match->insertField(std::move(ethTypeField));

            // Send this to the brain for a prediction
            std::unique_ptr<FlowStats> flowStatsCopy = std::unique_ptr<FlowStats>(new FlowStats());

            flowStatsCopy->crc = crc;
            flowStatsCopy->durationNSec = 0;
            flowStatsCopy->packetCount = 0;
            flowStatsCopy->byteCount = 0;
            flowStatsCopy->index = *index;


            entry->setIdleTimeout(3);
            entry->setHardTimeout(3);
            entry->setPriority(10000);
            entry->setBufferId(OFP_NO_BUFFER);
            entry->setOutGroup(OFPG_ANY);
            entry->setFlags(1);
            entry->setOutPort(port);
            uint32_t importance = nn.predict(*flowStatsCopy);
            if(importance <= 0){
                entry->setImportance(1);
            }
            else
            {
                entry->setImportance(importance);
            }


            std::unique_ptr <OpenFlow::OpenFlowActionOutput> outputAction
                    = std::unique_ptr<OpenFlow::OpenFlowActionOutput>(new OpenFlow::OpenFlowActionOutput(port));

            std::unique_ptr <OpenFlow::OpenFlowInstruction> instruction
                    = std::unique_ptr<OpenFlow::OpenFlowInstruction>(
                            new OpenFlow::OpenFlowInstruction(OFPIT_APPLY_ACTIONS));

            instruction->insertAction(std::move(outputAction));

            std::unique_ptr <OpenFlow::OpenFlowSetL2Src> srcMacModAction
                    = std::unique_ptr<OpenFlow::OpenFlowSetL2Src>(new OpenFlow::OpenFlowSetL2Src(srcMac));

            instruction->insertAction(std::move(srcMacModAction));

            std::unique_ptr <OpenFlow::OpenFlowSetL2Dst> dstMacModAction
                    = std::unique_ptr<OpenFlow::OpenFlowSetL2Dst>(new OpenFlow::OpenFlowSetL2Dst(dstMac));

            instruction->insertAction(std::move(dstMacModAction));
            match->insertInstruction(std::move(instruction));
            entry->insertMatchCriteria(std::move(match));
            len = entry->buildPacketData(txBuf);
            m_flowTable.insert(std::make_pair(crc, std::move(entry)));

        }
        // else
        {
            //return 0;
        }

        return len;
    }

    void OpenFlowTable::addFlowResults(uint32_t crc, uint32_t durationNSec, uint64_t packetCount, uint64_t byteCount)
    {
        // Find the flow stats entry
        auto search = m_flowStatsByCRC.find(crc);
        if(search != m_flowStatsByCRC.end())
        {
            std::unique_ptr<FlowStats> flowStatsCopy = std::unique_ptr<FlowStats>(new FlowStats());
            flowStatsCopy->timestamp = search->second->timestamp;
            std::cout << " Timestamp: " << flowStatsCopy->timestamp << "\n";

            flowStatsCopy->crc = crc;
            flowStatsCopy->durationNSec = durationNSec;
            flowStatsCopy->packetCount = packetCount;
            flowStatsCopy->byteCount = byteCount;
            flowStatsCopy->index = search->second->index;
            nn.learn(*flowStatsCopy);
            m_flowResults.push_back(std::move(flowStatsCopy));

            m_flowStatsByCRC.erase(search);

        }
        else
        {
            std::cout << "Error: Flow Stats Entry Not Found\n";
        }

    }

    void OpenFlowTable::removeStatsFromCache(uint32_t crc)
    {
        auto search = m_flowStatsByCRC.find(crc);
        if(search != m_flowStatsByCRC.end())
        {
            m_flowStatsByCRC.erase(search);
        }
        else
        {
            std::cout << "Tried to remove a non existing stats entry\n";
        }
    }

    void OpenFlowTable::addFlowStatsToCache(uint32_t crc, Network::FlowIndexV4 *index)
    {
        auto search = m_flowStatsByCRC.find(crc);

        if(index != NULL && search == m_flowStatsByCRC.end())
        {

            std::unique_ptr<FlowStats> flowStats = std::unique_ptr<FlowStats>(new FlowStats());
            std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()
            );

            flowStats->index = *index;
            flowStats->timestamp = ms.count();

            m_flowStatsByCRC.insert(std::pair<uint32_t, std::unique_ptr<FlowStats>>(crc, std::move(flowStats)));

        }
    }
}
