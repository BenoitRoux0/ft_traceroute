#include "ft_traceroute.h"

struct addrinfo* parse_host(char* name) {
    struct addrinfo     hints;
    struct addrinfo*    addrinfo = NULL;
    struct sockaddr_in  sockaddr;

    ft_bzero(&hints, sizeof (struct addrinfo));
    ft_bzero(&sockaddr, sizeof (struct sockaddr_in));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_NUMERICSERV;

    int status = getaddrinfo(name, NULL, &hints, &addrinfo);

    if (status) {
        dprintf(2, "error: getaddrinfo: %s\n", gai_strerror(status));
        return NULL;
    }

    return addrinfo;
}
