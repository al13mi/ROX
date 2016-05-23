#include "OpenFlowTable.h"

#include "Messages/OxmTLV.h"
#include "Messages/HeaderEncoder.h"
#include "Messages/HelloDecoder.h"
#include "Messages/FeaturesDecoder.h"
#include "Messages/FlowMatchEncoder.h"
#include "Messages/PacketInDecoder.h"
#include "Messages/FlowModEncoder.h"
#include "Messages/FlowModInstructionEncoder.h"
#include "Messages/FlowModActionEncoder.h"

/**
namespace OpenFlow {
    void OpenFlowActionOutput::buildPacketData(uint8_t *startPtr, uint8_t *endPtr)
    {
        OpenFlow::Messages::FlowModActionEncoder action(startPtr);
        action.setType(OpenFlow::Messages::FlowModActionEncoder::OFPAT_OUTPUT);
        action.setOutputPort(m_port);
        action.setMaxLen(0xffff);
        action.setPadding();
        action.setActionLen(action.getLength());
        endPtr += action.getLength();
    }

    OpenFlowActionOutput::OpenFlowActionOutput(uint32_t port, uint32_t maxLength)
        : m_port(port)
        , m_maxLength(maxLength)
    {
    }

    void OpenFlowInstruction::buildPacketData(uint8_t *startPtr, uint8_t *endPtr)
    {
        OpenFlow::Messages::FlowModInstructionEncoder instruction(startPtr);
        instruction.setType(OpenFlow::Messages::FlowModInstructionEncoder::OFPIT_APPLY_ACTIONS);
        instruction.setPadding();
        endPtr = startPtr + instruction.getLength();
        uint8_t *ptr = endPtr;

        std::list<std::unique_ptr<OpenFlowAction>>::iterator it;
        for (it = m_action.begin(); it != m_action.end(); ++it)
        {
            *it->buildPacketData(ptr, ptr);
        }

        endPtr = ptr;
    }

    void OpenFlowInstruction::insertAction(OpenFlowAction *action)
    {
        m_action.push_front(action);
    }

    OpenFlowInstruction::OpenFlowInstruction(uint16_t type)
        : m_type(type)
    {

    }


    void OpenFlowMatchFields::buildPacketData(uint8_t *start, uint8_t *end)
    {
        OpenFlow::Messages::OxmTLV tlv(start);
        tlv.setOxmClass(OpenFlow::Messages::OxmTLV::OFPXMC_OPENFLOW_BASIC); // 16
        tlv.setOxmField(field);
        tlv.setOxmValue(value);
        tlv.setOxmLength(4);
        flowMatchEncoder.setFlowMatchLength(12);
        end = start + 12;
    }

    OpenFlowMatchFields::OpenFlowMatchFields(uint16_t oxmClass, uint8_t field, uint32_t value)
        : m_oxmClass(oxmClass)
        , m_field(field)
        , m_value(value)
    {
    }



    OpenFlowMatch::OpenFlowMatch(
            uint32_t xid,
            uint8_t version,
            uint16_t priority,
            uint32_t outPort,
            uint64_t cookie,
            uint64_t cookieMask,
            uint16_t idleTimeout,
            uint16_t hardTimeout
    )
        : m_xid(xid)
        , m_version(version)
        , m_priority(priority)
        , m_outPort(outPort)
        , m_cookie(cookie)
        , m_cookieMask(cookieMask)
        , m_idleTimeout(idleTimeout)
        , m_hardTimeout(hardTimeout)

    {
    }

    void OpenFlowMatch::buildPacketData(uint8_t *start, uint8_t *end)
    {
        OpenFlow::Messages::FlowModEncoder flowModEncoder(flowModStart);
        flowModEncoder.setType(OpenFlow::Messages::HeaderEncoder::OFPT_FLOW_MOD);
        flowModEncoder.setXid(m_xid);
        flowModEncoder.setVersion(m_version);

        flowModEncoder.setCookie(m_cookie);
        flowModEncoder.setCookieMask(m_cookieMask);
        flowModEncoder.setTableId(0);
        flowModEncoder.setCommand(OpenFlow::Messages::FlowModEncoder::OFPFC_ADD);
        flowModEncoder.setIdleTimeout(m_idleTimeout);
        flowModEncoder.setHardTimeout(m_hardTimeout);
        flowModEncoder.setPriority(m_priority);
        flowModEncoder.setBufferId(OFP_NO_BUFFER);
        flowModEncoder.setOutGroup(OpenFlow::Messages::FlowModEncoder::OFPG_ANY);
        flowModEncoder.setFlags(0);
        flowModEncoder.setOutPort(m_outPort);

        uint8_t* match = flowModEncoder.getMatchFieldWritePtr();
        OpenFlow::Messages::FlowMatchEncoder flowMatchEncoder(match);
        flowMatchEncoder.setFlowMatchType(OpenFlow::Messages::FlowMatchEncoder::OFPMT_OXM);
        uint8_t* ptr = flowMatchEncoder.getOxmFields();

        std::list<std::unique_ptr<OpenFlowMatchFields>>::iterator it;
        for (it = m_fields.begin(); it != m_fields.end(); ++it)
        {
            *it->buildPacketData(ptr, ptr);
        }

        std::list<std::unique_ptr<OpenFlowInstruction>>::iterator instructionIt;
        for (instructionIt = m_instruction.begin(); it != m_instruction.end(); ++instructionIt)
        {
            *it->buildPacketData(ptr, ptr);
        }

        endPtr = ptr;
    }

    void OpenFlowMatch::insertField(OpenFlowMatchFields *field)
    {
        m_fields.push_front(field);
    }

    void OpenFlowMatch::insertInstruction(OpenFlowInstruction *instruction)
    {
        m_instruction.push_front(instruction);
    }

 **/
//}
