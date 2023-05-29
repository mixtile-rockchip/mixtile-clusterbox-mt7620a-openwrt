ifndef CFLAGS
CFLAGS = -O2 -g -I ./
endif
OBJS = ch343_demo.o ch343_lib.o

all: ch343-gpio

ch343_demo.o: ch343_demo.c
	$(CC) $(CFLAGS) -c -o $@ $<

ch343_lib.o: ch343_lib.c
	$(CC) $(CFLAGS) -c -o $@ $<

ch343-gpio: $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f ch343-gpio *.o
