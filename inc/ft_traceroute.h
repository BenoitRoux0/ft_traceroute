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
# include <error.h>

# ifdef BONUS
#  include <argp.h>
#  include <stdint.h>
# endif

# define DEFAULT_PORT (33434)
# define DEFAULT_WAIT (2)
# define DEFAULT_MAX_TTL (64)
# define DEFAULT_START_TTL (1)
# define DEFAULT_TRIES (3)
# define DEFAULT_HOSTNAME (NULL)
# define DEFAULT_RESOLVE (false)

typedef struct trc_opts_s {
    uint16_t    port;
    uint8_t     start_ttl;
    uint8_t     max_ttl;
    u_int       wait;
    u_int       tries;
    char*       hostname;
    bool        resolve;
}   trc_opts_t;

typedef struct trc_s {
    trc_opts_t          opts;
    struct addrinfo*    addr;
    int                 send_sock;
    int                 recv_sock;
    fd_set              set;
    struct timeval      send_time;
    struct timeval      recv_time;
    bool                parse_failed;
}   trc_t;

# define DEFAULT_OPTS { DEFAULT_PORT, \
                        DEFAULT_START_TTL,\
                        DEFAULT_MAX_TTL, \
                        DEFAULT_WAIT, \
                        DEFAULT_TRIES,\
                        DEFAULT_HOSTNAME, \
                        DEFAULT_RESOLVE \
}

# define UDP_SEND_SOCKET (socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
# define ICMP_RECV_SOCKET (socket(AF_INET, SOCK_RAW, IPPROTO_ICMP))

// traceroute
int                 traceroute_loop(trc_t* trc);
struct addrinfo*    parse_host(char* name);
int                 send_probe(trc_t* trc, struct sockaddr* sockaddr, socklen_t* len);

// trc_t
int     init_trc(int ac, char **av, trc_t *trc);
void    clear_trc(trc_t *trc);

// utils
void    ft_bzero(void* ptr, size_t n);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
double  get_time_diff(trc_t *trc);
int     put_time(trc_t* trc);
int     put_num_host(const struct sockaddr* sockaddr, socklen_t len);

# ifdef BONUS
int parse_opt(int key, char* arg, struct argp_state* state);
# endif

#endif
