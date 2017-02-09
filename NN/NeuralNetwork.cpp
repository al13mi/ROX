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
            auto &waitScope = client.getWaitScope();
            for(it = statsListCopy.begin(); it != statsListCopy.end(); it++)
            {
                std::lock_guard <std::mutex> guard(nnLock);
                auto request = brain.learnRequest();
                auto requestPacket = request.initPacket();
                ::capnp::List<Brain::Entry>::Builder packet = requestPacket.initData(Network::FlowIndexV4::BYTE_KEY_SIZE);
                
                for(int n = 0; n<Network::FlowIndexV4::BYTE_KEY_SIZE; n++)
                {
                    Brain::Entry::Builder entry = packet[n];
                    uint8_t word = (*it)->index.contents.byteKey[n];
                    entry.setValue(word);
                }

                auto priority = request.initPriority();
                uint32_t packetCount = (*it)->packetCount;
                priority.setValue(packetCount);
                auto promise = request.send();
                auto response = promise.wait(waitScope);
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
        std::lock_guard <std::mutex> guard(nnLock);
        capnp::EzRpcClient client("127.0.0.1:3333");
        Brain::Client brain(client.getMain<Brain>());

        auto& waitScope = client.getWaitScope();
        {
            auto request = brain.predictRequest();
            auto requestPacket = request.initPacket();

            ::capnp::List<Brain::Entry>::Builder packet = requestPacket.initData(Network::FlowIndexV4::BYTE_KEY_SIZE);
            for(int n = 0; n<Network::FlowIndexV4::BYTE_KEY_SIZE; n++)
            {
                Brain::Entry::Builder entry = packet[n];
                uint8_t word = stats.index.contents.byteKey[n];
                entry.setValue(word);
            }
            
            auto evalPromise = request.send();
            auto readPromise = evalPromise.getPriority().readRequest().send();
            auto response = readPromise.wait(waitScope);
            return response.getValue();
        }
        
    }
}
