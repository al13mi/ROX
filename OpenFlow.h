#ifndef _OPENFLOW_H_
#define _OPENFLOW_H_

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
    uint32_t bitmaps[0];
};

struct ofp_hello
{
   struct ofp_header header;
   struct ofp_hello_elem_header elements[0];
};

#pragma pack()
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

