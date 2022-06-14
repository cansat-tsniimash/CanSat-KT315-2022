#ifndef SD_H_
#define SD_H_

#include <includes.h>
#include <radio.h>

uint16_t sd_parse_to_text_dosimeter(char *buffer, rf_dosimeter_package_crc_t data);
uint16_t sd_parse_to_text_bmp(char *buffer, rf_bmp_package_crc_t data);
uint16_t sd_parse_to_text_ds(char *buffer, rf_ds_package_crc_t data);
uint16_t sd_parse_to_text_gps(char *buffer, rf_gps_package_crc_t data);
uint16_t sd_parse_to_text_inertial(char *buffer, rf_inertial_package_crc_t data);
uint16_t sd_parse_to_text_sebastian(char *buffer, rf_sebastian_package_crc_t data);

#endif /* SD_H_ */
