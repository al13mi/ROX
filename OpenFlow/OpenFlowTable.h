#ifndef ROX_OPENFLOW_TABLE_H
#define ROX_OPENFLOW_TABLE_H

#include <map>
#include <list>

namespace OpenFlow {

    class PacketData {
    public:
        virtual void buildPacketData(uint8_t *buffer) = 0;
        virtual void buildTableData(uint8_t *buffer) = 0;
    };

    class OpenFlowAction : public PacketData {
        uint16_t type;
    };

    class OpenFlowActionOutput : public OpenFlowAction {
    public:
        virtual void buildPacketData(uint8_t *buffer);
        virtual void buildTableData(uint8_t *buffer);

    private:
        uint32_t port;
        uint16_t maxLength;
    };

    class OpenFlowInstruction : public PacketData {
    public:
        virtual void buildPacketData(uint8_t *buffer);
        virtual void buildTableData(uint8_t *buffer);
        virtual void insertAction(OpenFlowAction *action);
    private:
        uint16_t type;
        std::list <OpenFlowAction> m_action;
    };

    class OpenFlowMatchFields : public PacketData {
    public:
        virtual void buildPacketData(uint8_t *buffer);
        virtual void buildTableData(uint8_t *buffer);

    private:
        uint16_t oxmClass;
        uint8_t field;
    };

    class OpenFlowMatch : public PacketData {
    public:
        OpenFlowMatch(uint16_t type);

        virtual void buildPacketData(uint8_t *buffer);
        virtual void buildTableData(uint8_t *buffer);
        void insertField(OpenFlowMatchFields *field);
        void insertInstruction(OpenFlowInstruction *instruction);

    private:
        uint16_t type;
        std::list <OpenFlowMatchFields> m_fields;
        std::list <OpenFlowInstruction> m_instruction;
    };

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

        virtual void buildPacketData(uint8_t *buffer);
        virtual void buildTableData(uint8_t *buffer);
        void insertMatchCriteria(OpenFlowMatch *match);

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

        std::list <OpenFlowMatch> match;
    };


    class OpenFlowTable {
    public:
        OpenFlowTable();

        ~OpenFlowTable();

    private:
        std::map <OpenFlowTableEntry> m_flowTable;
    };
}
#endif