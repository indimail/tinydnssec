/*
 * (C) 2012 Peter Conrad <conrad@quisquis.de>
 *
 * program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef DNSSEC
#include "base32hex.h"

#define to_32hex(c) ((c) < 10 ? (c) + '0' : (c) + 'a' - 10)

/*
 * out must point to a buffer of at least (len * 8 / 5) + 1 bytes.
 * Encoded string is *not* padded.
 * See RFC-4648. This implementation produces lowercase hex characters.
 * Returns length of encoded string.
 */
unsigned int
base32hex(char *out, uint8_t * in, unsigned int len)
{
	int             buf = 0, bits = 0;
	char           *x = out;

	while (len-- > 0) {
		buf <<= 8;
		buf |= *in++;
		bits += 8;
		while (bits >= 5) {
			char            c = (buf >> (bits - 5)) & 0x1f;
			*x++ = to_32hex(c);
			bits -= 5;
		}
	}
	if (bits > 0) {
		char            c = (buf << (5 - bits)) & 0x1f;
		*x++ = to_32hex(c);
	}
	return x - out;
}

#ifdef TEST
#include <substdio.h>
#include <stdlib.h>
#include <str.h>
#include <strmsg.h>

static void
test(char *in, char *expected, int explen)
{
	char            buf[255], strnum1[FMT_ULONG], strnum2[FMT_ULONG];
	int             r;

	if ((r = base32hex(buf, in, str_len(in))) != explen) {
		strnum1[fmt_int(strnum1, r)] = 0;
		strnum2[fmt_int(strnum2, explen)] = 0;
		strmsg_out
		str_msgout7("Failed: b32h('", in, "') yields ", strnum1, " chars (expected ", strnum2, ")\n");
		_exit(1);
	}
	if (str_diffn(buf, expected, r)) {
		buf[r] = 0;
		str_msgout7("Failed: b32h('", in, "') = '", buf, "' (expected ", expected, ")\n");
		_exit(1);
	}
}

int
main(int argc, char **argv)
{
	test("", "", 0);
	test("f", "co", 2);
	test("fo", "cpng", 4);
	test("foo", "cpnmu", 5);
	test("foob", "cpnmuog", 7);
	test("fooba", "cpnmuoj1", 8);
	test("foobar", "cpnmuoj1e8", 10);
	str_msgout1("Success!\n");
	_exit(0);
}
#endif
#endif
