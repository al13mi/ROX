#ifndef _PYNET_H_
#define _PYNET_H_

#include "NeuralNetwork.h"
#include <mutex>
#include <thread>
#include <list>

#ifndef BUILD_TYPE
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include "brain.capnp.h"
#include <capnp/ez-rpc.h>

using ::Brain;
#endif

#include <iostream>

namespace OpenFlow
{
    struct FlowStats;
}

namespace Python {
    class TensorFlowRNN {
    public:
        TensorFlowRNN();

        uint32_t learn(const OpenFlow::FlowStats &stats);

        uint32_t predict(const OpenFlow::FlowStats &stats);

        void worker();

    private:
        std::list<std::unique_ptr<OpenFlow::FlowStats>> statsList;
        std::mutex nnLock;
        std::thread T;
    };
}
#endif
