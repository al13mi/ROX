#ifndef ERROR_H
#define ERROR_H

namespace System {

    enum class Error {
        NO_ERROR = 0,
        ERR_ROUTE_NOT_FOUND = 1,
        ERR_ARP_NOT_FOUND = 2,
        ERR_MATCH_NOT_FOUND = 3
    };
}

#endif