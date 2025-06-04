#include "ft_traceroute.h"

int main(int ac, char **av) {
    trc_t       trc;
    trc_opts_t  traceroute_opts = DEFAULT_OPTS;

    if (ac == 1)
        return 1;

    trc.send_sock = UDP_SEND_SOCKET;

    if (trc.send_sock == -1) {
        dprintf(2, "%s: send socket: %s\n", av[0], strerror(errno));
        return errno;
    }

    trc.recv_sock = ICMP_RECV_SOCKET;

    if (trc.recv_sock == -1) {
        dprintf(2, "%s: recv socket: %s\n", av[0], strerror(errno));
        return errno;
    }

    trc.opts = traceroute_opts;

    FD_ZERO(&trc.set);

    trc.addr = parse_host(av[1]);

    traceroute_loop(&trc);

    close(trc.send_sock);
    close(trc.recv_sock);
    freeaddrinfo(trc.addr);
}
