#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "Controller.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

using namespace std;

OpenFlow::Controller controller;

struct bufferevent *bev;

static void
controller_read_cb(struct bufferevent *bev, void *ctx)
{
    unsigned char buf[BUFFER_SIZE];
    struct evbuffer *input = bufferevent_get_input(bev);
    ev_ssize_t len = evbuffer_remove(input, &buf, BUFFER_SIZE);
    controller.rxPacket(buf, len);
}

static void
controller_event_cb(struct bufferevent *bev, short events, void *ctx)
{    
    if (events & BEV_EVENT_ERROR)
    {
            perror("Error from bufferevent");
    }
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
    {
            bufferevent_free(bev);
    }
}

static void
accept_conn_cb(struct evconnlistener *listener,
    evutil_socket_t fd, struct sockaddr *address, int socklen,
    void *ctx)
{
    /* We got a new connection! Set up a bufferevent for it. */
    struct event_base *base = evconnlistener_get_base(listener);
    bev = bufferevent_socket_new(
            base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, controller_read_cb, NULL, controller_event_cb, NULL);
    
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    controller.connectionHandler();



}

static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));
    
    event_base_loopexit(base, NULL);
}

void txPacket(unsigned char *buf, ssize_t size)
{
   struct evbuffer *output = bufferevent_get_output(bev);
   evbuffer_add(output, buf, size);
   bufferevent_flush(bev, EV_WRITE, BEV_FLUSH);
   // evbuffer_write(bev, buf, output);
}


#ifndef TESTMAIN
int
main(int argc, char **argv)
{
    struct event_base *base;
    struct evconnlistener *listener;
    struct sockaddr_in sin;
    
    base = event_base_new();
    if (!base)
    {
        puts("Couldn't open event base");
        return 1;
    }
    
    /* Clear the sockaddr before using it, in case there are extra
    * platform-specific fields that can mess us up. */
    memset(&sin, 0, sizeof(sin));
    /* This is an INET address */
    sin.sin_family = AF_INET;
    /* Listen on 0.0.0.0 */
    sin.sin_addr.s_addr = htonl(0);
    /* Listen on the given port. */
    sin.sin_port = htons(LISTEN_PORT);
    
    listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
    LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
        (struct sockaddr*)&sin, sizeof(sin));
    if (!listener)
    {
        perror("Couldn't create listener");
        return 1;
    }
    evconnlistener_set_error_cb(listener, accept_error_cb);
    
    event_base_dispatch(base);
    return 0;
}
#endif