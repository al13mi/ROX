#ifndef IF_INTERFACE_H
#define IF_INTERFACE_H

namespace System {
    class IfInterface {
    public:
        IfInterface(uint32_t ifid) {

        }
    private:
        void rxPacket(IpPacket *pkt);
        void txPacket(IpPacket *pkt);
    };
}
#endif