#include "sevenseg_util.h"
#include <ctype.h>

struct char_to_ss {
	char c;
	unsigned char seg;
};

/*
 *       0
 *     ====
 *  5||    ||1
 *   ||  6 ||
 *     ====
 *  4||    ||2
 *   ||  3 ||
 *     ====  (o)7
*/

const struct char_to_ss char_to_ss[] = {
	{ '0', 0b00111111 },
	{ '1', 0b00000110 },
	{ '2', 0b01011011 },
	{ '3', 0b01001111 },
	{ '4', 0b01100110 },
	{ '5', 0b01101101 },
	{ '6', 0b01111101 },
	{ '7', 0b00000111 },
	{ '8', 0b01111111 },
	{ '9', 0b01101111 },
	{ 'A', 0b01110111 }, // A
	{ 'B', 0b01111100 }, // b
	{ 'C', 0b00111001 }, // C
	{ 'D', 0b01011110 }, // d
	{ 'E', 0b01111001 }, // E
	{ 'F', 0b01110001 }, // F
	{ 'H', 0b01110110 }, // H
	{ 'I', 0b00000100 }, // i
	{ 'L', 0b00111000 }, // L
	{ 'N', 0b01010100 }, // n
	{ 'O', 0b01011100 }, // o
	{ 'P', 0b01110011 }, // P
	{ 'R', 0b01010000 }, // r
	{ 'T', 0b01111000 }, // t
	{ 'U', 0b00111110 }, // u
	{ '-', 0b01000000 }, // -
	{ '_', 0b00001000 }, // _
	{ '.', 0b10000000 }, // .
	{ '=', 0b01001000 }, // =
	{ '/', 0b01010010 }, // /
	{ '\\',0b01100100 }, // '\\'
	{ ' ', 0b00000000 },
	{ 0, 0 }
};

int
sevenseg_char_to_seg(char c, unsigned char *seg)
{
	const struct char_to_ss *p = char_to_ss;

	for (p = char_to_ss; p->c; p++) {
		if (c == p->c) {
			*seg = p->seg;
			return 0;
		}
	}
	return -1;
}

int
sevenseg_str_to_segs(char *str, unsigned char *segs, int segs_len){
	int i=0;

	while (*str && segs_len > 0){
		if (sevenseg_char_to_seg(toupper(*str), segs) == -1)
			sevenseg_char_to_seg('_', segs);
		/* special handling for '.' */
		if ( *(str+1) == '.' ) {
			*segs |= 0b10000000;
			str++;
		}
		str++;
		segs++;
		i++;
		segs_len--;
	}
	return i;
}