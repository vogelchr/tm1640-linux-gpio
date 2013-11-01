#include "tm1640_gpio.h"
#include "sysfs_gpio.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct tm1640_gpio {
	struct sysfs_gpio *gpio_sclk;
	struct sysfs_gpio *gpio_din;
};

static inline void bit_delay(void) { };

static int 
tm1640_gpio_start(struct tm1640_gpio *p)
{
	if (sysfs_gpio_value(p->gpio_din, 0))
		return -1;
        bit_delay();
	if (sysfs_gpio_value(p->gpio_sclk, 0))
		return -1;
        bit_delay();
	return 0;
}

static int
tm1640_gpio_end(struct tm1640_gpio *p)
{
	if (sysfs_gpio_value(p->gpio_din, 0))
		return -1;
        bit_delay();
	if (sysfs_gpio_value(p->gpio_sclk, 1))
		return -1;
        bit_delay();
	if (sysfs_gpio_value(p->gpio_din, 1))
		return -1;
        bit_delay();
	return 0;
}

static int
tm1640_gpio_send_byte(struct tm1640_gpio *p,unsigned char v)
{
        unsigned char b=0x01; /* start at LSB */
        while(b){
       		if (sysfs_gpio_value(p->gpio_din, !! (v & b)))
			return -1;
                bit_delay();
		if (sysfs_gpio_value(p->gpio_sclk, 1))
			return -1;
	        bit_delay();
		if (sysfs_gpio_value(p->gpio_sclk, 0))
			return -1;
                bit_delay();
                b <<= 1; /* move from LSB to MSB */
        }
        return 0;
}

int
tm1640_gpio_command(struct tm1640_gpio *p,unsigned char v)
{
        if (tm1640_gpio_start(p) ||
            tm1640_gpio_send_byte(p,v) ||
            tm1640_gpio_end(p))
        	return -1;
       	return 0;
}

int
tm1640_gpio_data(struct tm1640_gpio *p,const unsigned char *data, int len)
{
	int i;
	if (tm1640_gpio_start(p) || tm1640_gpio_send_byte(p,0x40))
		return -1;
	for (i=0; i<len; i++)
		if (tm1640_gpio_send_byte(p,data[i]))
			return -1;
	if (tm1640_gpio_end(p))
		return -1;
	return 0;
}

struct tm1640_gpio *
tm1640_gpio_init(int n_gpio_din, int n_gpio_sclk)
{
	struct tm1640_gpio *p = calloc(1, sizeof(*p));
	static const unsigned char empty_buf[16] = { 0 };

	p->gpio_sclk = sysfs_gpio_open(n_gpio_sclk);
	p->gpio_din  = sysfs_gpio_open(n_gpio_din);

	if (p->gpio_din == NULL || p->gpio_sclk == NULL)
		goto err_out;

	if (sysfs_gpio_direction(p->gpio_din, 1)  ||
	    sysfs_gpio_direction(p->gpio_sclk, 1) ||
	    sysfs_gpio_value(p->gpio_din, 1)      ||
	    sysfs_gpio_value(p->gpio_sclk, 1))
		goto err_out;

/*
	if (tm1640_gpio_command(p, TM1640_OFF) ||
	    tm1640_gpio_data(p, empty_buf, sizeof(empty_buf)) ||
	    tm1640_gpio_command(p, TM1640_PWM(7)))
		goto err_out;
*/

	return p;

err_out:
	fprintf(stderr, "%s(%d, %d): error during initialization.\n",
		__FUNCTION__,n_gpio_din, n_gpio_sclk);
	if (p)
		tm1640_gpio_close(p);
	return NULL;
}

void
tm1640_gpio_close(struct tm1640_gpio *p)
{
	if (p) {
		if(p->gpio_din)
			sysfs_gpio_close(p->gpio_din);
		if(p->gpio_sclk)
			sysfs_gpio_close(p->gpio_sclk);
		free(p);
	}
}