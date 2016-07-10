#ifndef _PYNET_H_
#define _PYNET_H_

#include "NeuralNetwork.h"
#include <Python.h>

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

    private:
        PyObject *pName, *pModule, *pDict, *pFunc;
        PyObject *pArgs, *pValue;
    };
}
#endif
