#include "buffer.h"

void buffer_init(buffer *s, long long (*op)(int, char *, unsigned long long), int fd, char *buf, unsigned long long len) {

    s->x  = buf;
    s->fd = fd;
    s->op = op;
    s->p  = 0;
    s->n  = len;
    return;
}
