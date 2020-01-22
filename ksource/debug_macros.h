#include <netdb.h>

#undef err
#define err(fmt, args...) \
    do { \
        fprintf(stderr, pr_fmt(fmt), "error", ##args); \
    } while (0)
#undef info
#define info(fmt, args...) \
    do { \
        fprintf(stderr, pr_fmt(fmt), "info", ##args); \
    } while (0)
#undef dbg
#define dbg(fmt, args...) \
    do { \
        fprintf(stderr, dbg_fmt(fmt), ##args); \
    } while (0)
