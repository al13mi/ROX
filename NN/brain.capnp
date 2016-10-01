@0x85150b117366d14b;

interface Brain {
  predict @0 (packet: Packet) -> (priority: Priority);
  learn @1 (packet: Packet, priority: Priority);

  struct Packet {
    data @0 : List(Entry);
  }

  struct Entry {
    value @0 : UInt8;
  }

  struct ActualPriority {
    value @0 : UInt16;
  }

  interface Priority {
        read @0 () ->(value :UInt16);
  }
}
