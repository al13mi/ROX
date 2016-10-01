import argparse
import socket
import random
import capnp

import brain_capnp

class PriorityImpl(brain_capnp.Brain.Priority.Server):
        def __init__(self, value):
            self.value = value

        def read(self, **kwargs):
            return self.value

class BrainImpl(brain_capnp.Brain.Server):
        def predict(self, packet, _context, **kwargs):
            print packet
            return PriorityImpl(5)
        def learn(self, packet, _context, **kwargs):
            print packet

def main():
        address = "127.0.0.1:3333"
        server = capnp.TwoPartyServer(address, bootstrap=BrainImpl())
        server.run_forever()

if __name__ =='__main__':
        main()
