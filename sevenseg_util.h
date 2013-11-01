#ifndef SEVENSEG_UTIL_H
#define SEVENSEG_UTIL_H

extern int
sevenseg_char_to_seg(char c, unsigned char *seg);

extern int
sevenseg_str_to_segs(char *str, unsigned char *segs, int segs_len);

#endif