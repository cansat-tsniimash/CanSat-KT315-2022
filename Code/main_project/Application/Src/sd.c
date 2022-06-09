#include <sd.h>
#include <includes.h>

#include <radio.h>

void parse_to_text_dose(rf_dosimeter_package_crc_t data) {
	char buf [500];
	uint16_t num_written = snprintf(buf, 500, data.pack.flag, ";", data.pack.num, ";", data.pack.time_from_start, ";", data.pack.ticks_now, ";", data.pack.ticks_sum, ";", data.crc, "\n");
	//char
	//memcpy()
}

void parse_to_text_bmp(rf_bmp_package_crc_t data) {

}

void parse_to_text_ds(rf_ds_package_crc_t data) {

}

void parse_to_text_gps(rf_gps_package_crc_t data) {

}

void parse_to_text_inertial(rf_inertial_package_crc_t data) {

}

void parse_to_text_sebastian(rf_sebastian_package_crc_t data) {

}








void write_sd_dose(FIL *file) {

	//f_write(file, (uint8_t*) testBuffer, sizeof(testBuffer), &testBytes);
	//f_sync(file);
}

void write_sd_bmp() {

}

void write_sd_gps() {

}

void write_sd_inertial() {

}

void write_sd_sebastian() {

}
