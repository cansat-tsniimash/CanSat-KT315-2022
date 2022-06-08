#ifndef GPS_H_
#define GPS_H_

#include <includes.h>

typedef struct {
	float gps_lat;
	float gps_lon;
	float gps_alt;
} gps_data_t;

void gps_init_stm32();

#endif /* GPS_H_ */
