#include "ft_traceroute.h"

int init_trc(char **av, trc_t *trc);

int main(int ac, char **av) {
    trc_t       trc;

    if (ac == 1)
        return 1;

    for (int i = 0; i < ac; ++i) {
        if (strncmp(av[i], "--help", 7) == 0)  {
            dprintf(1, "Help menu\n");
            return 0;
        }
    }

    init_trc(av, &trc);
    traceroute_loop(&trc);
    clear_trc(&trc);
}

int init_trc(char **av, trc_t *trc) {
    trc->send_sock = -1;
    trc->recv_sock = -1;
    trc->addr = NULL;

    trc->send_sock = UDP_SEND_SOCKET;
    if (trc->send_sock == -1) {
        dprintf(2, "%s: send socket: %s\n", av[0], strerror(errno));
        exit(errno);
    }

    trc->recv_sock = ICMP_RECV_SOCKET;
    if (trc->recv_sock == -1) {
        dprintf(2, "%s: recv socket: %s\n", av[0], strerror(errno));
        clear_trc(trc);
        exit(errno);
    }

    trc_opts_t  traceroute_opts = DEFAULT_OPTS;
    trc->opts = traceroute_opts;
    FD_ZERO(&trc->set);
    trc->addr = parse_host(av[1]);

    if (trc->addr == NULL) {
        clear_trc(trc);
        exit(errno);
    }

    return 0;
}
