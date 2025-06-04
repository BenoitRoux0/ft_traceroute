#include "ft_traceroute.h"

static int send_packet(trc_t* trc);
static int wait_res(trc_t* trc);
static int recv_res(trc_t* trc, struct sockaddr* sockaddr, socklen_t* len);
static int send_probe(trc_t* trc, struct sockaddr* sockaddr, socklen_t* len);
static int put_num_host(const struct sockaddr* sockaddr, socklen_t len);
static int put_time(trc_t* trc);
static double get_time_diff(trc_t *trc);

int traceroute_loop(trc_t* trc) {
    uint8_t         current_ttl = trc->opts.start_ttl;
    struct sockaddr addr;
    socklen_t       len = sizeof (addr);
    bool            reached = false;
    bool            host_printed;

    while (!reached && current_ttl < trc->opts.max_ttl) {
        setsockopt(trc->send_sock, IPPROTO_IP, IP_TTL, &current_ttl, sizeof (current_ttl));
        ((struct sockaddr_in *) trc->addr->ai_addr)->sin_port = htons(trc->opts.port);
        host_printed = false;

        for (int i = 0; i < trc->opts.tries; ++i) {
            if (send_probe(trc, &addr, &len)) {
                return -1;
            }

            if (i == 0) {
                dprintf(1, "%3d  ", current_ttl);
            }

            if (!FD_ISSET(trc->recv_sock, &trc->set)) {
                dprintf(1, " * ");
                continue;
            }

            if (!host_printed) {
                put_num_host((const struct sockaddr *) &addr, len);
                host_printed = true;
            }
            put_time(trc);
        }

        dprintf(1, "\n");
        reached = (((struct sockaddr_in *) trc->addr->ai_addr)->sin_addr.s_addr == ((struct sockaddr_in *) &addr)->sin_addr.s_addr);
        ++trc->opts.port;
        ++current_ttl;
    }

    return 0;
}

static int send_probe(trc_t* trc, struct sockaddr* sockaddr, socklen_t* len) {
    if (send_packet(trc)) {
        return -1;
    }

    if (wait_res(trc)) {
        return -1;
    }

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

    bzero(sockaddr, sizeof(struct sockaddr));

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

static int put_time(trc_t* trc) {
    double diff = get_time_diff(trc);

    return dprintf(1, " %.3fms ", diff);
}

static int put_num_host(const struct sockaddr* sockaddr, socklen_t len) {
    char num_buffer[17] = { 0 };

    int status = getnameinfo(sockaddr, len, num_buffer, 16, NULL, 0, NI_NUMERICHOST);

    if (status) {
        dprintf(2, "error: getnameinfo: %s\n", gai_strerror(status));
        return -1;
    }

    return dprintf(1, " %s ", num_buffer);
}

static double get_time_diff(trc_t *trc) {
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


