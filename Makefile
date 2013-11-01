

CC=clang
CFLAGS=-ggdb -Os -Wall -Wextra
OBJS=tm1640_util.o tm1640_gpio.o sysfs_gpio.o sevenseg_util.o

all : tm1640_util

tm1640_util : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.d : %.c
	$(CC) $(CPPFLAGS) -o $@ -MM $^

.PHONY : clean
clean :
	rm -f *~ *.o *.d tm1640_util

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif