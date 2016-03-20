#ifndef _OPENFLOW_H_
#define _OPENFLOW_H_

#define OFP_NO_BUFFER 0xffffffff

#pragma pack(1)
struct ofp_header
{
    uint8_t version;
    uint8_t type;
    uint16_t length;
    uint32_t xid;
};

struct ofp_hello_elem_header
{
    uint16_t type;
    uint16_t length;
};

struct ofp_hello_elem_versionbitmap
{
    uint16_t type;
    uint16_t length;
    uint32_t bitmaps[1];
};

struct ofp_hello
{
   struct ofp_header header;
   struct ofp_hello_elem_header elements[1];
};



#pragma pack()

enum ofp_instruction_type {
    OFPIT_GOTO_TABLE = 1,
    OFPIT_WRITE_METADATA = 2,
    OFPIT_WRITE_ACTIONS = 3,
    OFPIT_APPLY_ACTIONS = 4,
    OFPIT_CLEAR_ACTIONS = 5,
    OFPIT_METER = 6,
    OFPIT_EXPERIMENTER = 0xFFFF
};

enum ofp_flow_mod_command
{
    OFPFC_ADD               = 0,
    OFPFC_MODIFY            = 1,
    OFPFC_MODIFY_STRICT     = 2,
    OFPFC_DELETE            = 3,
    OFPFC_DELETE_STRICT     = 4,
};
enum ofp_hello_elem_type
{
    OFPHET_VERSIONBITMAP = 1,
};

enum types
{
    HELLO = 0,
    ECHO_REQUEST = 2,
    ECHO_REPLY = 3,
    FEATURES_REQUEST = 5,
    FEATURES_REPLY = 6,
};
   
#endif

