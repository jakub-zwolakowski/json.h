
int isatty(int fd) {
  return 0;
}

#include <time.h>

int clock_gettime(clockid_t clk_id, struct timespec *tp) {
  tp->tv_sec = 42;
  tp->tv_nsec = 42;
  return 0;
}
