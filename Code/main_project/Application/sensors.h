#ifndef SENSORS_H_
#define SENSORS_H_


//Func for counting CrcSum for package
unsigned short Crc16(unsigned char *buf, unsigned short len);

//Func for packing data that will be sent by radio
rf_package_crc_t pack(struct bme280_data *bmp_data, lsm_data_t *lsm_data, uint16_t package_num);

#endif /* SENSORS_H_ */
