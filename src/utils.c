#include "ft_traceroute.h"

int put_time(trc_t* trc) {
    double diff = get_time_diff(trc);

    return dprintf(1, " %.3fms ", diff);
}

int put_num_host(const struct sockaddr* sockaddr, socklen_t len) {
    char num_buffer[17] = { 0 };

    int status = getnameinfo(sockaddr, len, num_buffer, 16, NULL, 0, NI_NUMERICHOST);

    if (status) {
        dprintf(2, "error: getnameinfo: %s\n", gai_strerror(status));
        return -1;
    }

    return dprintf(1, " %s ", num_buffer);
}

double get_time_diff(trc_t *trc) {
    double          secs;
    double          usecs;

    secs = trc->recv_time.tv_sec - trc->send_time.tv_sec;
    usecs = trc->recv_time.tv_usec - trc->send_time.tv_usec;

    if (trc->recv_time.tv_usec < trc->send_time.tv_usec) {
        usecs += 1000000;
        secs -= 1;
    }

    return  usecs * 1. / 1000. + secs * 1000.;
}

void ft_bzero(void* ptr, size_t n) {
    uint8_t* s = ptr;

    if (ptr == NULL)
        return;

    for (size_t i = 0; i < n; ++i) {
        s[i] = 0;
    }
}

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
