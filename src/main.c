#include "ft_traceroute.h"

int main(int ac, char **av) {
    trc_t   trc;

    if (ac == 1)
        return 1;

    init_trc(ac, av, &trc);
    traceroute_loop(&trc);
    clear_trc(&trc);
}
