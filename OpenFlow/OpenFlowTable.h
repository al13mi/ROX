#ifndef ROX_OPENFLOW_TABLE_H
#define ROX_OPENFLOW_TABLE_H

#include <map>
#include <list>
#include <memory>

namespace OpenFlow {

    class PacketData {
    public:
        virtual void buildPacketData(uint8_t *startPtr, uint8_t *endPtr) = 0;
    };

    class OpenFlowAction : public PacketData {
        uint16_t type;
    };

    class OpenFlowActionOutput : public OpenFlowAction
    {
    public:
        virtual void buildPacketData(uint8_t *startPtr, uint8_t *endPtr);
        OpenFlowActionOutput(uint32_t port, uint32_t maxLength = 0xffff);

    private:
        uint32_t m_port;
        uint16_t m_maxLength;
    };

    class OpenFlowInstruction : public PacketData {
    public:
        OpenFlowInstruction(uint16_t type);
        virtual void buildPacketData(uint8_t *startPtr, uint8_t *endPtr);
        virtual void insertAction(std::unique_ptr<OpenFlowAction> action);
    private:
        uint16_t m_type;
        std::list <std::unique_ptr<OpenFlowAction>> m_action;
    };

    class OpenFlowMatchFields : public PacketData {
    public:
        OpenFlowMatchFields(uint16_t oxmClass, uint8_t field, uint32_t value);
        virtual void buildPacketData(uint8_t *startPtr, uint8_t *endPtr);

    private:
        uint16_t m_oxmClass;
        uint8_t m_field;
        uint32_t m_value;
    };


class OpenFlowMatch : public PacketData {
public:
    OpenFlowMatch(uint32_t xid,
                  uint8_t version,
                  uint16_t priority,
                  uint32_t outPort,
                  uint64_t cookie = 0,
                  uint64_t cookieMask = 0,
                  uint16_t idleTimeout = 0,
                  uint16_t hardTimeout = 0
    );

    virtual void buildPacketData(uint8_t *startPtr, uint8_t *endPtr);
    void insertField(std::unique_ptr<OpenFlowMatchFields> field);
    void insertInstruction(std::unique_ptr<OpenFlowInstruction> instruction);

private:
    // OpenFlow Header
    uint32_t m_xid;
    uint8_t m_version;

    uint16_t m_priority;
    uint32_t m_outPort;

    uint64_t m_cookie;
    uint64_t m_cookieMask;

    uint16_t m_idleTimeout;
    uint16_t m_hardTimeout;

    std::list <std::unique_ptr<OpenFlowMatchFields>> m_fields;
    std::list <std::unique_ptr<OpenFlowInstruction>> m_instruction;
};

    /**
class OpenFlowTableEntry : public PacketData {
public:
    OpenFlowTableEntry(uint64_t cookie, uint64_t cookieMask, uint8_t tableId);

    void setIdleTimeout(uint16_t idleTimeout);
    void setPriority(uint16_t idleTimeout);
    void setBufferId(uint32_t idleTimeout);
    void setOutPort(uint32_t outPort);
    void setOutGroup(uint32_t outGroup);
    void setFlags(uint16_t flags);
    void setImportance(uint16_t importance);

    virtual void buildPacketData(uint8_t *startPtr, uint8_t *endPtr);
    void insertMatchCriteria(std::unique_ptr<OpenFlowMatch> match);

private:
    uint64_t m_cookie;
    uint64_t m_cookieMask;

    uint8_t m_tableId;
    uint8_t m_command;
    uint16_t m_idleTimeout;
    uint16_t m_hardTimeout;
    uint16_t m_priority;
    uint32_t m_bufferId;
    uint32_t m_outPort;
    uint32_t m_outGroup;
    uint16_t m_flags;
    uint16_t m_importance;

    std::list <std::unique_ptr<OpenFlowMatch>> match;
};


class OpenFlowTable {
public:
    OpenFlowTable(){}

    ~OpenFlowTable(){}

private:
    std::map <uint64_t, std::unique_ptr<OpenFlowTableEntry>> m_flowTable;
};
**/

}
#endif