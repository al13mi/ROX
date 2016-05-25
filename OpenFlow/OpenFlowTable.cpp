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

extern void txPacket(unsigned char *buf, ssize_t size);

#define OFP_NO_BUFFER 0xffffffff

namespace OpenFlow {
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
        tlv.setOxmValue(m_value);
        tlv.setOxmLength(4);
        return 12;
    }

    OpenFlowMatchFields::OpenFlowMatchFields(uint16_t oxmClass, uint8_t field, uint32_t value)
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

        flowMatchEncoder.setFlowMatchLength(len-4);


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
                std::unique_ptr<OpenFlow::OpenFlowTableEntry>(new OpenFlow::OpenFlowTableEntry(0, 0, 0));
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
}
