#include "ft_traceroute.h"

void clear_trc(trc_t *trc) {
    if (trc->send_sock != -1) {
        close(trc->send_sock);
    }
    if (trc->recv_sock != -1) {
        close(trc->recv_sock);
    }
    if (trc->addr != NULL) {
        freeaddrinfo(trc->addr);
    }
}