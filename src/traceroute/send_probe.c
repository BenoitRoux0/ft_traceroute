#include "ft_traceroute.h"

static int send_packet(trc_t* trc);
static int wait_res(trc_t* trc);
static int recv_res(trc_t* trc, struct sockaddr* sockaddr, socklen_t* len);

int send_probe(trc_t* trc, struct sockaddr* sockaddr, socklen_t* len) {
    if (send_packet(trc))
        return -1;

    if (wait_res(trc))
        return -1;

    if (recv_res(trc, sockaddr, len)) {
        return -1;
    }

    return 0;
}


static int send_packet(trc_t* trc) {
    sendto(trc->send_sock, "SUPERMAN", 9, 0, trc->addr->ai_addr, trc->addr->ai_addrlen);

    gettimeofday(&trc->send_time, NULL);

    return 0;
}

static int wait_res(trc_t* trc) {
    FD_SET(trc->recv_sock, &trc->set);
    struct timeval  timeout;

    timeout.tv_sec = trc->opts.wait;
    timeout.tv_usec = 0;

    select(trc->recv_sock + 1, &trc->set, NULL, NULL, &timeout);
    gettimeofday(&trc->recv_time, NULL);

    return 0;
}

static int recv_res(trc_t* trc, struct sockaddr* sockaddr, socklen_t* len) {
    static uint8_t  buffer[1024];

    if (!FD_ISSET(trc->recv_sock, &trc->set)) {
        return 0;
    }

    ft_bzero(sockaddr, sizeof(struct sockaddr));

    for (int i = 0; i < 1024; ++i) {
        buffer[i] = 0;
    }

    ssize_t received = recvfrom(trc->recv_sock, buffer, 1024, 0, sockaddr, len);

    if (received == -1) {
        dprintf(2, "error: recvfrom: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}
