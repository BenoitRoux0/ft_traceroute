#include "ft_traceroute.h"

static int  print_start_msg(char* hostname, trc_t* trc);
static void init_sockets(char *const *av, trc_t *trc);
static void init_addr(trc_t *trc, char *hostname);
static void print_help(void);

int init_trc(int ac, char **av, trc_t *trc) {
    char *hostname;

    trc->send_sock = -1;
    trc->recv_sock = -1;
    trc->addr = NULL;

    for (int i = 0; i < ac; ++i) {
        if (ft_strncmp(av[i], "--help", 7) == 0)  {
            print_help();
            exit(0);
        } else {
            hostname = av[i];
        }
    }

    init_sockets(av, trc);

    trc_opts_t  traceroute_opts = DEFAULT_OPTS;
    trc->opts = traceroute_opts;
    FD_ZERO(&trc->set);

    init_addr(trc, hostname);
    print_start_msg(hostname, trc);

    return 0;
}

static void init_addr(trc_t *trc, char *hostname) {
    trc->addr = parse_host(hostname);

    if (trc->addr == NULL) {
        clear_trc(trc);
        exit(errno);
    }
}

static void init_sockets(char *const *av, trc_t *trc) {
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
}

static int  print_start_msg(char* hostname, trc_t* trc) {
    char    num_buffer[17] = { 0 };

    getnameinfo(trc->addr->ai_addr, trc->addr->ai_addrlen, num_buffer, 16, NULL, 0, NI_NUMERICHOST);

    return dprintf(1, "traceroute to %s (%s), %d hops max\n", hostname, num_buffer, trc->opts.max_ttl);
}

static void print_help(void) {
    dprintf(1, "Usage: ft_traceroute_bonus [OPTION...]\n");
    dprintf(1, "\n");
    dprintf(1, "      --help                 Give this help list\n");
    dprintf(1, "\n");
    dprintf(1, "Mandatory or optional arguments to long options are also mandatory or optional\n");
    dprintf(1, "for any corresponding short options.\n");
    dprintf(1, "\n");
    dprintf(1, "Report bugs to beroux@student.42lyon.fr.\n");
}
