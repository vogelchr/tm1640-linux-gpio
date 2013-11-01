#include "tm1640_gpio.h"
#include "sevenseg_util.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

                           /* Toradex t20 module, on a Iris carrier */
#define DEFAULT_DIN     29 /* gpio 29 == SODI-98, Iris X16-13 */ 
#define DEFAULT_SCLK    89 /* gpio 89 == SOD-103, Iris X16-15 */

void
usage(char *argv0)
{
	fprintf(stderr, "Usage: %s [options] [string]\n", argv0);
	fprintf(stderr, "  -s N    gpio pin for SCLK signal (def: %d)\n",
		DEFAULT_SCLK);
	fprintf(stderr, "  -d N    gpio pin for DIN signal (def: %d)\n",
		DEFAULT_DIN);
	fprintf(stderr, "  -p pos  position cursor on char #pos (0..15)\n");
	fprintf(stderr, "  -b N    brightness 0=off, 1..8 dark..bright\n");
	fprintf(stderr, "  -c      clear display up to eol\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	int sclk = DEFAULT_SCLK;
	int din = DEFAULT_DIN;
	int pos = 0;
	int bright = 1;
	int clear = 0;

	struct tm1640_gpio *p;
	unsigned char buf[16];
	int i;

	while ((i = getopt(argc, argv, "s:d:p:b:c")) != -1) {
		switch (i) {
		case 's':
			sclk = atoi(optarg);
			break;
		case 'd':
			din = atoi(optarg);
			break;
		case 'p':
			pos = atoi(optarg);
			break;
		case 'b':
			bright = atoi(optarg);
			if (bright < 0 || bright > 8) {
				fprintf(stderr,"Bad brightness value!\n");
				usage(argv[0]);
				break;
			}
			break;
		case 'c':
			clear++;
			break;
		default:
			usage(argv[0]);
			break;
		}
	}

	p = tm1640_gpio_init(din, sclk);
	if (!p) {
		fprintf(stderr, "Could not initialize tm1640 on GPIOs.\n");
		exit(1);
	}

	/* doesn't seem to work without this command? I'm confused. */
	tm1640_gpio_command(p, TM1640_ADDR(0) );


	fprintf(stderr,"Setting brightness to %d.\n", bright);
	if (bright == 0)
		tm1640_gpio_command(p, TM1640_OFF);
	else
		tm1640_gpio_command(p, TM1640_PWM(bright-1));

	if (argc > optind) { /* there's an command-line argument */
		fprintf(stderr,"Writing \"%s\"...\n", argv[optind]);
		i = sevenseg_str_to_segs(argv[optind], buf, sizeof(buf));
	} else {
		fprintf(stderr,"Writing nothign.\n");
		i = 0;
	}

	if (clear) {
		while (i+pos < sizeof(buf))
			buf[i++] = 0;
	}

	if (i) {
		fprintf(stderr,"Writing %d bytes at pos %d.\n", i, pos);
		tm1640_gpio_command(p, TM1640_ADDR(pos) );
		tm1640_gpio_data(p, buf, i);
	}
	
	return 0;
}