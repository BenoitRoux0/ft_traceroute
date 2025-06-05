#include "ft_traceroute.h"

static int  print_start_msg(char* hostname, trc_t* trc);
static void init_sockets(char *const *av, trc_t *trc);
static void init_addr(trc_t* trc, char *hostname);

int init_trc(int ac, char **av, trc_t* trc) {
    trc->send_sock = -1;
    trc->recv_sock = -1;
    trc->addr = NULL;
    trc->parse_failed = false;

    struct argp_option  options[] = {
            {"first-hop", 'f', "NUM", 0, "set initial hop distance, i.e., time-to-live", 0},
            {"max-hop", 'm', "NUM", 0, "set maximal hop count (default: 64)", 0},
            {"port", 'p', "PORT", 0, "use destination PORT port (default: 33434)", 0},
            {"tries", 't', "NUM", 0, "send NUM probe packets per hop (default: 3)", 0},
            {"wait", 'w', "NUM", 0, "wait NUM seconds for response (default: 3)", 0},
            {"resolve-hostnames", 300, NULL, 0, "resolve hostnames", 0},
            { 0 }
    };

    trc_opts_t  traceroute_opts = DEFAULT_OPTS;
    trc->opts = traceroute_opts;

    struct argp argp = {options, parse_opt, NULL, NULL, NULL, NULL, NULL};

    argp_parse(&argp, ac, av, 0, NULL, &trc->opts);

    init_sockets(av, trc);

    FD_ZERO(&trc->set);

    init_addr(trc, trc->opts.hostname);
    print_start_msg(trc->opts.hostname, trc);

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
