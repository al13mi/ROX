#include "NN/NeuralNetwork.h"
#include "OpenFlow/OpenFlowTable.h"
#include <iostream>
namespace Python {

    TensorFlowRNN::TensorFlowRNN() {
        Py_SetProgramName((char*)"PyNNRunner");
        Py_Initialize();

        pName = PyString_FromString((char*)"TensorFlowRNN");
        pModule = PyImport_Import(pName);

        if(pModule != NULL)
        {
            std::cout << "Attempting to initialize Python Interpreter\n";
            PyObject *pFunc = PyObject_GetAttrString(pModule, (char*)"test_initialization");
            if (pFunc && PyCallable_Check(pFunc)) {
                PyObject_CallObject(pFunc, NULL);
            }
        }

        OpenFlow::FlowStats stats;

        stats.crc = 1;
        stats.durationNSec = 2;
        stats.packetCount = 3;
        stats.byteCount = 4;
        stats.timestamp = 5;

        stats.index.contents.ip.protocol = 6;
        stats.index.contents.ip.dscp = 7;
        stats.index.contents.ip.srcIp = 8;
        stats.index.contents.ip.destIp = 9;
        stats.index.contents.ip.protocolSpecific.tcpUdp.destPort = 10;
        stats.index.contents.ip.protocolSpecific.tcpUdp.srcPort = 11;

        learn(stats);
    }

    uint32_t TensorFlowRNN::learn(const OpenFlow::FlowStats &stats)
    {
        PyObject *pArgs = PyTuple_New(6);
        PyTuple_SetItem(pArgs, 0, PyLong_FromUnsignedLong(stats.crc));
        PyTuple_SetItem(pArgs, 1, PyLong_FromUnsignedLong(stats.durationNSec));
        PyTuple_SetItem(pArgs, 2, PyLong_FromUnsignedLongLong(stats.packetCount));
        PyTuple_SetItem(pArgs, 3, PyLong_FromUnsignedLongLong(stats.byteCount));
        PyTuple_SetItem(pArgs, 4, PyLong_FromUnsignedLongLong(stats.timestamp));

        // Take out the header information and pass it into a tuple
        PyObject *header = PyTuple_New(6);

        PyTuple_SetItem(header, 0, PyLong_FromUnsignedLong(stats.index.contents.ip.protocol));
        PyTuple_SetItem(header, 1, PyLong_FromUnsignedLong(stats.index.contents.ip.dscp));
        PyTuple_SetItem(header, 2, PyLong_FromUnsignedLong(stats.index.contents.ip.srcIp));
        PyTuple_SetItem(header, 3, PyLong_FromUnsignedLong(stats.index.contents.ip.destIp));
        PyTuple_SetItem(header, 4, PyLong_FromUnsignedLong(stats.index.contents.ip.protocolSpecific.tcpUdp.destPort));
        PyTuple_SetItem(header, 5, PyLong_FromUnsignedLong(stats.index.contents.ip.protocolSpecific.tcpUdp.srcPort));

        PyTuple_SetItem(pArgs, 5, header);

        PyObject *pFunc = PyObject_GetAttrString(pModule, (char*)"learn");
        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject_CallObject(pFunc, pArgs);
        }
        return 0;
    }

    uint32_t TensorFlowRNN::predict(const OpenFlow::FlowStats &stats)
    {

        PyObject *pArgs = PyTuple_New(6);
        PyTuple_SetItem(pArgs, 0, PyLong_FromUnsignedLong(stats.crc));
        PyTuple_SetItem(pArgs, 1, PyLong_FromUnsignedLong(stats.durationNSec));
        PyTuple_SetItem(pArgs, 2, PyLong_FromUnsignedLongLong(stats.packetCount));
        PyTuple_SetItem(pArgs, 3, PyLong_FromUnsignedLongLong(stats.byteCount));
        PyTuple_SetItem(pArgs, 4, PyLong_FromUnsignedLongLong(stats.timestamp));

        // Take out the header information and pass it into a tuple
        PyObject *header = PyTuple_New(6);

        PyTuple_SetItem(header, 0, PyLong_FromUnsignedLong(stats.index.contents.ip.protocol));
        PyTuple_SetItem(header, 1, PyLong_FromUnsignedLong(stats.index.contents.ip.dscp));
        PyTuple_SetItem(header, 2, PyLong_FromUnsignedLong(stats.index.contents.ip.srcIp));
        PyTuple_SetItem(header, 3, PyLong_FromUnsignedLong(stats.index.contents.ip.destIp));
        PyTuple_SetItem(header, 4, PyLong_FromUnsignedLong(stats.index.contents.ip.protocolSpecific.tcpUdp.destPort));
        PyTuple_SetItem(header, 5, PyLong_FromUnsignedLong(stats.index.contents.ip.protocolSpecific.tcpUdp.srcPort));

        PyTuple_SetItem(pArgs, 5, header);

        PyObject *pFunc = PyObject_GetAttrString(pModule, (char*)"learn");
        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject_CallObject(pFunc, pArgs);
        }

        return 0;
    }
}