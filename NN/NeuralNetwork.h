#ifndef _PYNET_H_
#define _PYNET_H_

#include "NeuralNetwork.h"
#include <Python.h>
#include <mutex>
#include <thread>
#include <list>
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
        PyObject *pName, *pModule;
        std::list<std::unique_ptr<OpenFlow::FlowStats>> statsList;
        std::mutex nnLock;
        std::thread T;

    };
}
#endif
