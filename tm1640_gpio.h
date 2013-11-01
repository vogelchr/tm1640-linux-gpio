#ifndef TM1640_GPIO_H
#define TM1640_GPIO_H

/* commands */
#define TM1640_ADDR(v)  (0xc0 | ((v)&0x0f)) /* move cursor to digit v */
#define TM1640_PWM(v)   (0x88 | ((v)&0x07)) /* PWM 0=1/16, 7=14/16 */
#define TM1640_OFF       0x80

/* Bit values map to digit segments (and decimal dot)
 * according to the following drawing:
 *
 *       0
 *     ====
 *  5||    ||1
 *   ||  6 ||
 *     ====
 *  4||    ||2
 *   ||  3 ||
 *     ====  (o)7
 */

#define TM1640_0     0b00111111
#define TM1640_1     0b00000110
#define TM1640_2     0b01011011
#define TM1640_3     0b01001111
#define TM1640_4     0b01100110
#define TM1640_5     0b01101101
#define TM1640_6     0b01111101
#define TM1640_7     0b00000111
#define TM1640_8     0b01111111
#define TM1640_9     0b01101111
#define TM1640_A     0b01110111
#define TM1640_B     0b01111100
#define TM1640_C     0b00111001
#define TM1640_D     0b01011110
#define TM1640_E     0b01111001
#define TM1640_F     0b01110001
#define TM1640_MINUS 0b00100000
#define TM1640_DP    0b10000000


struct tm1640_gpio;

/* open a tm1640 connected to the given GPIO pins, /sys/class/gpio/gpio<N>/
   must be accessible, will return -1 on error, 0 on success, some half-useful
   stuff will be written to stderr on failure */
extern struct tm1640_gpio *
tm1640_gpio_init(int n_gpio_din, int n_gpio_sclk);

/* close the tm1640 and associated IOs */
extern void
tm1640_gpio_close(struct tm1640_gpio *p);

/* send a command to the TM1640, see defines on top of this header file,
   return -1 on failure, 0 on success */
extern int
tm1640_gpio_command(struct tm1640_gpio *p,unsigned char v);

/* send data to TM1640, cursor has been positioned with TM1640_ADDR
   preceeding this call, returns -1 on failure, 0 on success */
extern int
tm1640_gpio_data(struct tm1640_gpio *p,const unsigned char *data, int len);

#endif