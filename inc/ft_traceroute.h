#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <netdb.h>
# include <stdlib.h>
# include <linux/ip.h>
# include <linux/icmp.h>
# include <sys/time.h>

# define DEFAULT_PORT (33434)
# define DEFAULT_WAIT (2)
# define DEFAULT_MAX_TTL (64)
# define DEFAULT_START_TTL (1)
# define DEFAULT_TRIES (3)

typedef struct trc_opts_s {
    uint16_t    port;
    uint8_t     start_ttl;
    uint8_t     max_ttl;
    int         wait;
    int         tries;
}   trc_opts_t;

typedef struct trc_s {
    trc_opts_t          opts;
    struct addrinfo*    addr;
    int                 send_sock;
    int                 recv_sock;
    fd_set              set;
    struct timeval      send_time;
    struct timeval      recv_time;
}   trc_t;

# define DEFAULT_OPTS {DEFAULT_PORT, DEFAULT_START_TTL, DEFAULT_MAX_TTL, DEFAULT_WAIT, DEFAULT_TRIES}

# define UDP_SEND_SOCKET (socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
# define ICMP_RECV_SOCKET (socket(AF_INET, SOCK_RAW, IPPROTO_ICMP))

int traceroute_loop(trc_t* trc);
struct addrinfo* parse_host(char* name);

#endif
