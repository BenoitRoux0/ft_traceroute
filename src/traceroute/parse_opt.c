#include "ft_traceroute.h"

static ulong parse_ulong(const struct argp_state *state, const char* arg, ulong max);

int parse_opt(int key, char* arg, struct argp_state* state) {
    trc_t*  trc = state->input;

    if (trc->parse_failed) {
        return 0;
    }

    switch (key) {
        case ARGP_KEY_ARG:
            trc->opts.hostname = arg;
            break;
        case 'f':
            trc->opts.start_ttl = parse_ulong(state, arg, UINT16_MAX);
            break;
        case 'p':
            trc->opts.port = parse_ulong(state, arg, UINT16_MAX);
            break;
        case 'q':
            trc->opts.tries = parse_ulong(state, arg, 10);
            break;
        case 'w':
            trc->opts.wait = parse_ulong(state, arg, UINT_MAX);
            break;
        case 300:
            trc->opts.resolve = true;
            break;
    }

    return 0;
}

static ulong parse_ulong(const struct argp_state *state, const char* arg, ulong max) {
    char*           end;
    trc_t*          trc = state->input;

    ulong val = strtoul(arg, &end, 10);

    if (*end != 0) {
        error(0, 0, "invalid value (`%s' near `%s')\n", arg, end);
        trc->parse_failed = true;
        return -1;
    }

    if (val > max) {
        error(0, 0, "option value too big: %s\n", arg);
        trc->parse_failed = true;
        return -2;
    }

    return val;
}