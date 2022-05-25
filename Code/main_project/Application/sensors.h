#ifndef SENSORS_H_
#define SENSORS_H_

typedef struct {
	double temperature;
	double pressure;
} bmp_data_t;

typedef struct {
	float temperature;
	float mag[3];
} lis_data_t;

typedef struct {
	float temperature;
	float acc[3];
	float gyro[3];
} lsm_data_t;

typedef struct {
	float gps_lat;
	float gps_lon;
	float gps_alt;

} gps_data_t;

sensors_t app_work();

#endif /* SENSORS_H_ */
