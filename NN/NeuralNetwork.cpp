#include "NN/NeuralNetwork.h"
#include "OpenFlow/OpenFlowTable.h"
#include <iostream>

namespace Python {


    void TensorFlowRNN::worker()
    {
        capnp::EzRpcClient client("127.0.0.1:3333");
        Brain::Client brain(client.getMain<Brain>());
            
        while(true)
        {
            std::list <std::unique_ptr<OpenFlow::FlowStats>> statsListCopy;
            {
                std::lock_guard <std::mutex> guard(nnLock);
                statsListCopy = std::move(statsList);
            }

            std::list <std::unique_ptr<OpenFlow::FlowStats>>::iterator it;
            for(it = statsListCopy.begin(); it != statsListCopy.end(); it++)
            {
                auto& waitScope = client.getWaitScope();
                {
                    std::cout << "Asking Python Brain something remotely\n";
                    std::cout.flush();
                    auto request = brain.predictRequest();
                    auto requestPacket = request.initPacket();
                    ::capnp::List<Brain::Entry>::Builder packet = requestPacket.initData(1);
                    Brain::Entry::Builder entry = packet[0];
                    entry.setValue(128);

                    auto evalPromise = request.send();
                    auto readPromise = evalPromise.getPriority().readRequest().send();
                    auto response = readPromise.wait(waitScope);
                    std::cout << response.getValue() << "\n";
                }
            }

            sleep(1);
        }
    }

    TensorFlowRNN::TensorFlowRNN()
    {

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
        return 0;
    }
}
