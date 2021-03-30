#include "fmt.h"
#include "byte.h"
#include "ip4.h"
#include "ip6.h"

/*
 * authors fefe, Erwin Hoffman
 */

unsigned int
ip6_fmt(char *s, const char ip[16])
{
	unsigned int    len, i, temp, temp0, compressing, compressed;
	int             j;

	len = compressing = compressed = 0;
	for (j = 0; j < 16; j += 2) {
		if (j == 12 && ip6_isv4mapped(ip)) {
			len += ip4_fmt(s, ip + 12);
			break;
		}
		temp = ((unsigned long) (unsigned char) ip[j] << 8) + (unsigned long) (unsigned char) ip[j + 1];
		temp0 = 0;
		if (!compressing && j < 16)
			temp0 = ((unsigned long) (unsigned char) ip[j + 2] << 8) + (unsigned long) (unsigned char) ip[j + 3];
		if (temp == 0 && temp0 == 0 && !compressed) {
			if (!compressing) {
				compressing = 1;
				if (j == 0) {
					if (s)
						*s++ = ':';
					++len;
				}
			}
		} else {
			if (compressing) {
				compressing = 0;
				++compressed;
				if (s)
					*s++ = ':';
				++len;
			}
			i = fmt_xlong(s, temp);
			len += i;
			if (s)
				s += i;
			if (j < 14) {
				if (s)
					*s++ = ':';
				++len;
			}
		}
	}
	if (compressing) {
		*s++ = ':';
		++len;
	}

	return len;
}

unsigned int
ip6_fmt_flat(char *s, const char ip[16])
{
	int             i;

	if (!s)
		return (32);
	for (i = 0; i < 16; i++) {
		*s++ = tohex((unsigned char) ip[i] >> 4);
		*s++ = tohex((unsigned char) ip[i] & 15);
	}
	return 32;
}
