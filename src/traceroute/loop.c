#include "ft_traceroute.h"

static int send_all_probes(trc_t *trc, uint8_t current_ttl, struct sockaddr *addr, socklen_t *len);

int traceroute_loop(trc_t* trc) {
    uint8_t         current_ttl = trc->opts.start_ttl;
    struct sockaddr_in addr;
    socklen_t       len = sizeof (addr);
    bool            reached = false;

    while (!reached && current_ttl < trc->opts.max_ttl) {
        setsockopt(trc->send_sock, IPPROTO_IP, IP_TTL, &current_ttl, sizeof (current_ttl));
        ((struct sockaddr_in *) trc->addr->ai_addr)->sin_port = htons(trc->opts.port);

        if (send_all_probes(trc, current_ttl, (struct sockaddr *) &addr, &len))
            return -1;

        reached = (((struct sockaddr_in *) trc->addr->ai_addr)->sin_addr.s_addr == addr.sin_addr.s_addr);
        ++trc->opts.port;
        ++current_ttl;
    }

    return 0;
}

static int send_all_probes(trc_t *trc, uint8_t current_ttl, struct sockaddr *addr, socklen_t *len) {
    bool host_printed = false;

    for (int i = 0; i < trc->opts.tries; ++i) {
        if (send_probe(trc, addr, len))
            return -1;

        if (i == 0)
            dprintf(1, "%3d  ", current_ttl);

        if (!FD_ISSET(trc->recv_sock, &trc->set)) {
            dprintf(1, " * ");
            continue;
        }

        if (!host_printed) {
            put_num_host((const struct sockaddr *) addr, (*len));
            host_printed = true;
        }

        put_time(trc);
    }

    dprintf(1, "\n");
    return 0;
}
