#ifndef GPS_H_
#define GPS_H_

#include <includes.h>

typedef struct {
	float longtitude;
	float latitude;
	float altitude;
	uint64_t time_sec;
	uint32_t time_microsec;
	uint8_t fix;
} gps_data_t;

void gps_init_stm32(void);
void gps_parse_buffer(void);
bool gps_get_data(gps_data_t *gps_data);


#endif /* GPS_H_ */
