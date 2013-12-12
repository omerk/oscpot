CC=gcc
CFLAGS=-Wall 

all: pihwm oscpot

pihwm:
	$(CC) $(CFLAGS) -c pihwm/lib/pihwm.c
	$(CC) $(CFLAGS) -c pihwm/lib/pi_spi.c

oscpot: pihwm
	$(CC) $(CFLAGS) -o oscpot -llo main.c pihwm.o pi_spi.o

debug: pihwm
	$(CC) $(CFLAGS) -DDEBUG -o oscpot -llo main.c pihwm.o pi_spi.o

deps:
	git clone https://github.com/omerk/pihwm.git
	sudo apt-get install liblo7 liblo-dev liblo-tools

clean:
	rm -f *.o oscpot

.PHONY: pihwm
