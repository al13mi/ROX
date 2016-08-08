#include "NN/NeuralNetwork.h"
#include "OpenFlow/OpenFlowTable.h"
#include <iostream>
namespace Python {


    void TensorFlowRNN::worker()
    {
        while(true)
        {
            // Make a copy so we can work on it.
            std::list <std::unique_ptr<OpenFlow::FlowStats>> statsListCopy;
            {
                std::lock_guard <std::mutex> guard(nnLock);
                statsListCopy = std::move(statsList);
            }

            std::list <std::unique_ptr<OpenFlow::FlowStats>>::iterator it;
            for(it = statsListCopy.begin(); it != statsListCopy.end(); it++)
            {
                PyGILState_STATE gilstate = PyGILState_Ensure();
                PyObject *pArgs = PyTuple_New(6);

                PyTuple_SetItem(pArgs, 0, PyLong_FromUnsignedLong((*it)->crc));
                PyTuple_SetItem(pArgs, 1, PyLong_FromUnsignedLong((*it)->durationNSec));
                PyTuple_SetItem(pArgs, 2, PyLong_FromUnsignedLongLong((*it)->packetCount));
                PyTuple_SetItem(pArgs, 3, PyLong_FromUnsignedLongLong((*it)->byteCount));
                PyTuple_SetItem(pArgs, 4, PyLong_FromUnsignedLongLong((*it)->timestamp));

                // Take out the header information and pass it into a tuple
                PyObject *header = PyTuple_New(6);

                PyTuple_SetItem(header, 0, PyLong_FromUnsignedLong((*it)->index.contents.ip.protocol));
                PyTuple_SetItem(header, 1, PyLong_FromUnsignedLong((*it)->index.contents.ip.dscp));
                PyTuple_SetItem(header, 2, PyLong_FromUnsignedLong((*it)->index.contents.ip.srcIp));
                PyTuple_SetItem(header, 3, PyLong_FromUnsignedLong((*it)->index.contents.ip.destIp));
                PyTuple_SetItem(header, 4,
                                PyLong_FromUnsignedLong((*it)->index.contents.ip.protocolSpecific.tcpUdp.destPort));
                PyTuple_SetItem(header, 5,
                                PyLong_FromUnsignedLong((*it)->index.contents.ip.protocolSpecific.tcpUdp.srcPort));

                PyTuple_SetItem(pArgs, 5, header);
                PyObject *pFunc = PyObject_GetAttrString(pModule, (char *) "learn");
                if (pFunc && PyCallable_Check(pFunc)) {
                    PyObject_CallObject(pFunc, pArgs);
                }

                if (PyErr_Occurred()) {
                    PyErr_Print();
                }
                PyGILState_Release(gilstate);

            }

            //sleep(5);
        }
    }

    TensorFlowRNN::TensorFlowRNN() {
        const char *scriptDirectoryName = "/home/adtran/ROX/NN";
        Py_Initialize();
        PyObject *sysPath = PySys_GetObject("path");
        PyObject *path = PyString_FromString(scriptDirectoryName);
        int result = PyList_Insert(sysPath, 0, path);

        Py_SetProgramName((char*)"PyNNRunner");

        PyGILState_STATE gilState = PyGILState_Ensure();
        pName = PyString_FromString((char*)"TensorFlowRNN");
        pModule = PyImport_ImportModule("TensorFlowRNN");
        if (PyErr_Occurred()) {
            PyErr_Print();
        }

        if(pModule != NULL)
        {
            std::cout << "Attempting to initialize Python Interpreter\n";
            PyObject *pFunc = PyObject_GetAttrString(pModule, (char*)"test_initialization");
            if (pFunc && PyCallable_Check(pFunc)) {
                PyObject_CallObject(pFunc, NULL);
            }
        }
        PyEval_ReleaseLock();

        std::thread thr(&TensorFlowRNN::worker, this);
        std::swap(thr, T);
    }

    uint32_t TensorFlowRNN::learn(const OpenFlow::FlowStats &stats)
    {
        std::lock_guard<std::mutex> guard(nnLock);
        std::unique_ptr <OpenFlow::FlowStats> statsCopy
                = std::unique_ptr<OpenFlow::FlowStats>(new OpenFlow::FlowStats());

        statsCopy->crc = stats.crc;
        statsCopy->durationNSec = stats.durationNSec;
        statsCopy->packetCount = stats.packetCount;
        statsCopy->byteCount = stats.byteCount;
        statsCopy->timestamp = stats.timestamp;
        statsCopy->index = stats.index;

        statsList.push_front(std::move(statsCopy));
        return 0;
    }

    uint32_t TensorFlowRNN::predict(const OpenFlow::FlowStats &stats)
    {
        PyGILState_STATE gilstate = PyGILState_Ensure();
        PyObject *pArgs = PyTuple_New(2);
        PyTuple_SetItem(pArgs, 0, PyLong_FromUnsignedLong(stats.crc));

        // Take out the header information and pass it into a tuple
        PyObject *header = PyTuple_New(6);
        PyTuple_SetItem(header, 0, PyLong_FromUnsignedLong(stats.index.contents.ip.protocol));
        PyTuple_SetItem(header, 1, PyLong_FromUnsignedLong(stats.index.contents.ip.dscp));
        PyTuple_SetItem(header, 2, PyLong_FromUnsignedLong(stats.index.contents.ip.srcIp));
        PyTuple_SetItem(header, 3, PyLong_FromUnsignedLong(stats.index.contents.ip.destIp));
        PyTuple_SetItem(header, 4, PyLong_FromUnsignedLong(stats.index.contents.ip.protocolSpecific.tcpUdp.destPort));
        PyTuple_SetItem(header, 5, PyLong_FromUnsignedLong(stats.index.contents.ip.protocolSpecific.tcpUdp.srcPort));

        PyTuple_SetItem(pArgs, 1, header);

        PyObject *pFunc = PyObject_GetAttrString(pModule, (char*)"predict");
        PyObject *pResult;

        unsigned long result = 0;
        if (pFunc && PyCallable_Check(pFunc)) {
            pResult = PyObject_CallObject(pFunc, pArgs);
            //result = PyInt_AsUnsignedLongMask(pResult);
        }
        else
        {
            std::cout << ":(\n";
        }
        if (PyErr_Occurred()) {
            PyErr_Print();
        }

        PyGILState_Release(gilstate);

        return (uint32_t)result;
    }
}