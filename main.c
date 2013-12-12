#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pihwm/lib/pihwm.h"
#include "lo/lo.h"

#ifdef DEBUG
	#define debug(...) printf(__VA_ARGS__)
#else
	#define debug(...) ;
#endif


int
mcp3002_read (int fd, int channel)
{
	uint8_t tx[3];
	uint8_t rx[3];

	if ( channel == 0 || channel == 1 ){
		tx[0] = 1;						// start bit
		tx[1] = (2 + channel) << 6;		// (sgl/diff) (odd/sign) (msbf)
										// odd/sign select channel in single ended mode
		tx[2] = 0;						// dummy data to shift result

		spi_transfer(fd, tx, rx, 3);

		return ((rx[1] & 31) << 6) + (rx[2] >> 2);
	} else {
		return -1;
	}
}


int
main ()
{
	int fd, val;

	// init SPI channel 0
	fd = spi_init(0);
	if (fd == -1){
		debug("Can't initialize SPI, are you running this as root?\n");
		exit(EXIT_FAILURE);
	}
	spi_config_default(fd);

	// the address of the OSC server	
	lo_address t = lo_address_new("127.0.0.1", "7770");

	for (;;){
		val = mcp3002_read(fd, 0);

		debug("Channel 0: %d, Channel 1: %d\n", mcp3002_read(fd, 0), mcp3002_read(fd, 1));
		
		if (lo_send(t, "/val", "i", val) == -1) {
			debug("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
		}

		delay(1000);
	}

	return 0;
}

