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

<<<<<<< HEAD
sensors_t app_work();
=======
void app_work();
>>>>>>> 8a7e5a19c2d7f0e0e4f4e7611d544e2697a211c4

#endif /* SENSORS_H_ */
