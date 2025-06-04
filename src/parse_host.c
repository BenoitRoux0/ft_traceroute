#include "ft_traceroute.h"

static void ft_bzero(void* s, size_t n);
//static void	*ft_memcpy(void *dst, const void *src, size_t n);

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
        exit(1);
    }

    return addrinfo;
}

static void ft_bzero(void* ptr, size_t n) {
    uint8_t* s = ptr;

    if (ptr == NULL)
        return;

    for (size_t i = 0; i < n; ++i) {
        s[i] = 0;
    }
}
//
//static void	*ft_memcpy(void *dst, const void *src, size_t n) {
//    size_t				i;
//    unsigned char		*dest;
//    const unsigned char	*source;
//
//    i = 0;
//    dest = (unsigned char *) dst;
//    source = (const unsigned char *) src;
//    if (!dst && !src)
//        return (NULL);
//    while (i < n) {
//        dest[i] = source[i];
//        i++;
//    }
//    return (dest);
//}