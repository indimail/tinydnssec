#include "e.h"
#include "str.h"
#include "byte.h"
#include "buffer.h"

static int allwrite(long long (*op)(int, const char *, unsigned long long), int fd, const char *buf, unsigned long long len) {

    long long w;

    if (!buf || len < 0) { errno = EINVAL; return -1; }

    while (len > 0) {
        w = op(fd, buf, len);
        if (w == -1) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN) continue;
            if (errno == EWOULDBLOCK) continue;
            return -1;
        }
        buf += w;
        len -= w;
    }
    return 0;
}


int buffer_flush(buffer *s) {

    long long p;

    p = s->p;
    if (!p) return 0;
    s->p = 0;
    return allwrite((long long (*)(int, const char *, unsigned long long)) s->op, s->fd, s->x, p);
}

int buffer_putalign(buffer *s, const char *buf, unsigned long long len) {

    long long n;

    if (!buf || len < 0) { errno = EINVAL; return -1; }

    while (len > (n = s->n - s->p)) {
        byte_copy(s->x + s->p, n, buf); s->p += n; buf += n; len -= n;
        s->p += n; buf += n; len -= n;
        if (buffer_flush(s) == -1) return -1;
    }
    /* now len <= s->n - s->p */
    byte_copy(s->x + s->p, len, buf);
    s->p += len;
    return 0;
}


int buffer_put(buffer *s, const char *buf, unsigned long long len) {

    long long n;

    if (!buf || len < 0) { errno = EINVAL; return -1; }

    n = s->n;
    if (len > n - s->p) {
        if (buffer_flush(s) == -1) return -1;
        /* now s->p == 0 */ 
        if (n < BUFFER_OUTSIZE) n = BUFFER_OUTSIZE;
        while (len > s->n) {
            if (n > len) n = len;
            if (allwrite((long long (*)(int, const char *, unsigned long long)) s->op, s->fd, buf, n) == -1) return -1;
            buf += n;
            len -= n;
        }
    }
    /* now len <= s->n - s->p */
    byte_copy(s->x + s->p, len, buf);
    s->p += len;
    return 0;
}

int buffer_putflush(buffer *s, const char *buf, unsigned long long len) {
    if (buffer_flush(s) == -1) return -1;
    return allwrite((long long (*)(int, const char *, unsigned long long)) s->op, s->fd, buf, len);
}

int buffer_putsalign(buffer *s, const char *buf) {
    return buffer_putalign(s, buf, str_len(buf));
}

int buffer_puts(buffer *s, const char *buf) {
    return buffer_put(s, buf, str_len(buf));
}

int buffer_putsflush(buffer *s, const char *buf) {
    return buffer_putflush(s, buf, str_len(buf));
}
