#include <app_main.h>
#include <includes.h>
#include <timers.h>
#include <status.h>

#include <shift_regs.h>

#include <bmp280.h>
#include <lis3mdl.h>
#include <lsm6ds3.h>
#include <ds18b20.h>
#include <photores.h>
#include <gps.h>
#include <sebastian.h>

#include <radio.h>
#include <sd.h>

extern SPI_HandleTypeDef hspi2;
extern ADC_HandleTypeDef hadc1;

/*struct minmea_sentence_gga {
    struct minmea_time time;
    struct minmea_float latitude;
    struct minmea_float longitude;
    int fix_quality;
    int satellites_tracked;
    struct minmea_float hdop;
    struct minmea_float altitude; char altitude_units;
    struct minmea_float height; char height_units;
    int dgps_age;
};*/
/* End data structures */


void app_main (void) {


	/* Begin Init */

	//SHIFT_REG
	shift_reg_t shift_reg_imu = shift_reg_create_descriptor(&hspi2, GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_13, 0);
	shift_reg_init_stm32(&shift_reg_imu);

	shift_reg_t shift_reg_rf = shift_reg_create_descriptor(&hspi2, GPIOC, GPIO_PIN_4, GPIOC, GPIO_PIN_5, 0);
	shift_reg_init_stm32(&shift_reg_rf);

	//BMP280
	bme_spi_intf_sr bmp_setup = bmp280_create_descriptor(2, &hspi2, &shift_reg_imu);
	struct bme280_dev bmp280 = bmp280_init(&bmp_setup);

	//DS18B20
	ds18b20_t ds = ds_create_descriptor(GPIOA, GPIO_PIN_1);
	ds_init(&ds);

	//LIS3MDL
	lis_spi_intf_sr lis_setup = lis_create_descriptor(3, &hspi2, &shift_reg_imu);
	stmdev_ctx_t lis = lis_init(&lis_setup);

	//LSM6DS3
	lsm_spi_intf_sr lsm_setup = lsm_create_descriptor(4, &hspi2, &shift_reg_imu);
	stmdev_ctx_t lsm = lsm_init(&lsm_setup);

	//Photoresistors
	adc_init(&hadc1);
	photoresistor_t photores_rckt = photores_create_descriptor(ANALOG_TARGET_ROCKET_CHECKER, &hadc1, 2000, 5);
	photoresistor_t photores_seed = photores_create_descriptor(ANALOG_TARGER_SEED_CHECKER, &hadc1, 2000, 5);

	//GPS
	gps_init_stm32();

	//NRF24
	nrf24_lower_api_config_t nrf24_config = {0};
	nrf24_service_t nrf24 = { .nrf24_lower_api_config = &nrf24_config };

	nrf24_spi_pins_sr_t nrf24_sr_setup = nrf24_create_sr_descriptor(&shift_reg_rf, 0, 1);
	nrf24_rf_config_t nrf24_rf_setup = nrf24_create_rf_descriptor(NRF24_DATARATE_250_KBIT, NRF24_TXPOWER_MINUS_0_DBM, 115);
	nrf24_protocol_config_t nrf24_protocol_setup = nrf24_create_protocol_descriptor(NRF24_CRCSIZE_DISABLE, NRF24_ADDRES_WIDTH_5_BYTES, true, true, true, 0, 0);
	nrf24_pipe_config_t nrf24_pipe_setup = nrf24_create_pipe_descriptor(false, 0xacacacacac, -1);
	nrf24_init_stm32(&nrf24_config, &hspi2, &nrf24_sr_setup, &nrf24_rf_setup, &nrf24_protocol_setup, &nrf24_pipe_setup);

	//SD-Card
	FATFS file_system;
	FIL defaults;
	FIL dosimeter_file;
	FIL bmp_file;
	FIL ds_file;
	FIL gps_file;
	FIL inertial_file;
	FIL sebastian_file;

	char sd_buffer[300] = {0};
	uint16_t sd_buffer_size = 0;
	UINT sd_bytes_written = 0;
	UINT sd_bytes_read = 0;
	FRESULT fres = 0;

	const char defaults_file_path[] = "defaults.bin";
	const char dosimeter_file_path[] = "dosimeter.csv";
	const char bmp_file_path[] = "bmp.csv";
	const char ds_file_path[] = "ds.csv";
	const char gps_file_path[] = "gps.csv";
	const char inertial_file_path[] = "inertial.csv";
	const char sebastian_file_path[] = "sebastian.csv";

	file_system_mount(&file_system);

	file_open(&file_system, &dosimeter_file, dosimeter_file_path, FA_WRITE | FA_OPEN_APPEND);
	file_puts(&file_system, &dosimeter_file, dosimeter_file_path, "flag;num;time from start;tps;tpm;ticks sum;crc\n");

	file_open(&file_system, &bmp_file, bmp_file_path, FA_WRITE | FA_OPEN_APPEND);
	file_puts(&file_system, &bmp_file, bmp_file_path, "flag;num;time from start;bmp temperature;bmp pressure;status;crc\n");

	file_open(&file_system, &ds_file, ds_file_path, FA_WRITE | FA_OPEN_APPEND);
	file_puts(&file_system, &ds_file, ds_file_path, "flag;num;time from start;ds temperature;lux rocket;lux seed;crc\n");

	file_open(&file_system, &gps_file, gps_file_path, FA_WRITE | FA_OPEN_APPEND);
	file_puts(&file_system, &gps_file, gps_file_path, "flag;num;time from start;longtitude;latitude;altitude;time sec high; time sec low;time microsec;fix;crc\n");

	file_open(&file_system, &inertial_file, inertial_file_path, FA_WRITE | FA_OPEN_APPEND);
	file_puts(&file_system, &inertial_file, inertial_file_path, "flag;num;time from start;acc x;acc y;acc z;gyro x;gyro y;gyro z;mag x;mag y;mag z;crc\n");

	file_open(&file_system, &sebastian_file, sebastian_file_path, FA_WRITE | FA_OPEN_APPEND);
	file_puts(&file_system, &sebastian_file, sebastian_file_path, "flag;num;time from start; quaternion 1; quaternion 2; quaternion 3; quaternion 4;crc\n");

	/* End Init */


	/* Begin data structures */

	uint32_t dosimeter_tps = 0;
	uint32_t dosimeter_tpm = 0;
	uint32_t dosimeter_sum = 0;

	bmp_data_t bmp_data = {0};

	float ds_temperature = 0.0;
	float photores_rckt_lux = 0.0;
	float photores_seed_lux = 0.0;

	lsm_data_t lsm_data = {0};
	lis_data_t lis_data = {0};
	int16_t lsm_acc[3] = {0};
	int16_t lsm_gyro[3] = {0};
	int16_t lis_mag[3] = {0};

	gps_data_t gps_data = {0};

	float seb_delta = 0;
	float seb_quaternion [4] = {0};

	status_t status = STATUS_BEFORE_ROCKET;
	uint8_t reboot_counter = 0;

	/* End data structures */


	/* Begin defaults */

	double ground_pressure = 0.0;
	double sum_pressure = 0.0;
	float ground_lux_rckt = 0.0;
	float sum_rckt = 0.0;
	for (uint8_t i = 0; i < 10; i++) {
		bmp_data = bmp280_get_data(&bmp280);
		ground_pressure = bmp_data.pressure;
		ground_lux_rckt = photores_get_data(photores_rckt);
		sum_pressure += ground_pressure;
		sum_rckt += ground_lux_rckt;
	}
	ground_pressure = sum_pressure / 10.0;
	ground_lux_rckt = sum_rckt / 10.0;

	sd_defaults_t default_data = {
		.ground_pressure = ground_pressure,
		.ground_lux_rckt = ground_lux_rckt,
		.status = status,
		.reboot_counter = reboot_counter
	};
	sd_defaults_crc_t default_data_crced = {
		.pack = default_data,
		.crc = Crc16((uint8_t*)&default_data, sizeof(default_data))
	};
	sd_defaults_crc_t read_defaults_data = {0};
	uint16_t read_crc = 0;

	fres = f_open(&defaults, defaults_file_path, FA_WRITE | FA_CREATE_NEW);
	if (FR_EXIST == fres) {
		file_open(&file_system, &defaults, defaults_file_path, FA_READ | FA_OPEN_EXISTING);
		file_read(&file_system, &defaults, defaults_file_path, &read_defaults_data, sizeof(read_defaults_data), &sd_bytes_read);
		read_crc = Crc16((uint8_t*)&read_defaults_data.pack, sizeof(read_defaults_data.pack));
		if (read_defaults_data.crc == read_crc) {
			ground_pressure = read_defaults_data.pack.ground_pressure;
			ground_lux_rckt = read_defaults_data.pack.ground_lux_rckt;
			status = read_defaults_data.pack.status;
			if (STATUS_AFTER == status) {
				status = STATUS_LANDED;
			}
			reboot_counter = read_defaults_data.pack.reboot_counter;
		} else {
			ground_pressure = 0.0;
			ground_lux_rckt = 0.0;
			status = STATUS_LANDED;
			update_status_in_defaults(&file_system, &defaults, defaults_file_path, &sd_bytes_written, &default_data_crced, status, reboot_counter);
		}
		reboot_counter++;
	} else if (FR_OK == fres) {
		file_write(&file_system, &defaults, defaults_file_path, &default_data_crced, sizeof(default_data_crced), &sd_bytes_written);
		file_sync(&file_system, &defaults, defaults_file_path);
	}
	f_close(&defaults);

	/* End defaults */


	/* Begin status checker */

	uint32_t time_on = 0;
	uint32_t time_out = 0;
	uint32_t time_burning = 0;
	float height = 0.0;
	float prev_height = 0.0;
	uint32_t time_height = 0;
	uint32_t time_prev_height = 0;
	uint8_t height_delta_counter = 0;

	/* Begin status checker */

	// Eternal loop
	while(true) {
		// Work~~ OwO

		//Dosimeter
		if (timecheck_dosimeter()) {
			dosimeter_tps = Dosimeter_Get_TPS();
			dosimeter_tpm = Dosimeter_Get_TPM();
			dosimeter_sum = Dosimeter_Get_Sum();

			rf_dosimeter_package_crc_t dosimeter_package = pack_rf_dosimeter(dosimeter_tps, dosimeter_tpm, dosimeter_sum);
			send_rf_package(&nrf24, &dosimeter_package, sizeof(dosimeter_package));

			sd_buffer_size = sd_parse_to_bytes_dosimeter(sd_buffer, &dosimeter_package);
			file_write(&file_system, &dosimeter_file, dosimeter_file_path, sd_buffer, sd_buffer_size, &sd_bytes_written);
		}

		//BMP280
		bmp_data = bmp280_get_data(&bmp280);

		rf_bmp_package_crc_t bmp_package = pack_rf_bmp(bmp_data.temperature, bmp_data.pressure, (uint8_t)status);
		send_rf_package(&nrf24, &bmp_package, sizeof(bmp_package));

		sd_buffer_size = sd_parse_to_bytes_bmp(sd_buffer, &bmp_package);
		file_write(&file_system, &bmp_file, bmp_file_path, sd_buffer, sd_buffer_size, &sd_bytes_written);

		//DS18B20
		if (timecheck_ds18b20()) {
			photores_rckt_lux = photores_get_data(photores_rckt);
			photores_seed_lux = photores_get_data(photores_seed);
			ds_temperature = ds_get_data(&ds);
			timer_update_ds18b20();

			rf_ds_package_crc_t ds_package = pack_rf_ds(ds_temperature, photores_rckt_lux, photores_seed_lux);
			send_rf_package(&nrf24, &ds_package, sizeof(ds_package));

			sd_buffer_size = sd_parse_to_bytes_ds(sd_buffer, &ds_package);
			file_write(&file_system, &ds_file, ds_file_path, sd_buffer, sd_buffer_size, &sd_bytes_written);
		}

		//GPS
		if (gps_get_data(&gps_data)) {
			rf_gps_package_crc_t gps_package = pack_rf_gps(gps_data.longtitude, gps_data.latitude, gps_data.altitude, (uint32_t)gps_data.time_sec, gps_data.time_microsec, gps_data.fix);
			send_rf_package(&nrf24, &gps_package, sizeof(gps_package));

			sd_buffer_size = sd_parse_to_bytes_gps(sd_buffer, &gps_package, gps_data.time_sec);
			file_write(&file_system, &gps_file, gps_file_path, sd_buffer, sd_buffer_size, &sd_bytes_written);
		}

		//Inertial
		lsm_data = lsm_get_data(&lsm);
		lis_data = lis_get_data(&lis);
		seb_delta = sebastian_get_delta();
		timer_update_sebastian();
		for (int i = 0; i < 3; i++) lsm_acc[i] = (int16_t)(lsm_data.acc[i] * 2000);
		for (int i = 0; i < 3; i++) lsm_gyro[i] = (int16_t)(lsm_data.gyro[i] * 15);
		for (int i = 0; i < 3; i++) lis_mag[i] = (int16_t)(lis_data.mag[i] * 2000);

		rf_inertial_package_crc_t inertial_package = pack_rf_inertial(lsm_acc, lsm_gyro, lis_mag);
		send_rf_package(&nrf24, &inertial_package, sizeof(inertial_package));

		sd_buffer_size = sd_parse_to_bytes_inertial(sd_buffer, &inertial_package, lsm_data.acc, lsm_data.gyro, lis_data.mag);
		file_write(&file_system, &inertial_file, inertial_file_path, sd_buffer, sd_buffer_size, &sd_bytes_written);

		//Sebastian Madgwick
		MadgwickAHRSupdate(seb_quaternion, lsm_data.gyro[0], lsm_data.gyro[1], lsm_data.gyro[2], lsm_data.acc[0], lsm_data.acc[1], lsm_data.acc[2], -1 * lis_data.mag[0], -1 * lis_data.mag[1], -1 * lis_data.mag[2], seb_delta, 0.3);

		rf_sebastian_package_crc_t sebastian_package = pack_rf_sebastian(seb_quaternion);
		send_rf_package(&nrf24, &sebastian_package, sizeof(sebastian_package));

		sd_buffer_size = sd_parse_to_bytes_sebastian(sd_buffer, &sebastian_package);
		file_write(&file_system, &sebastian_file, sebastian_file_path, sd_buffer, sd_buffer_size, &sd_bytes_written);

		//SD Sync
		if (timecheck_sd()) {
			file_sync(&file_system, &dosimeter_file, dosimeter_file_path);
			file_sync(&file_system, &bmp_file, bmp_file_path);
			file_sync(&file_system, &ds_file, ds_file_path);
			file_sync(&file_system, &gps_file, gps_file_path);
			file_sync(&file_system, &inertial_file, inertial_file_path);
			file_sync(&file_system, &sebastian_file, sebastian_file_path);
			timer_update_sd_sync();
		}

		//Check status
		if (STATUS_BEFORE_ROCKET == status) {
			photores_rckt_lux = photores_get_data(photores_rckt);
			if (!HAL_GPIO_ReadPin(Switch_GPIO_Port, Switch_Pin)) {
				time_on = HAL_GetTick();
			}
			if ((photores_rckt_lux < PHOTORESISTOR_CRITICAL_MODIFICATOR * ground_lux_rckt) && (HAL_GetTick() - time_on > 10000) && (HAL_GPIO_ReadPin(Switch_GPIO_Port, Switch_Pin))) {
				status = STATUS_IN_ROCKET;
				update_status_in_defaults(&file_system, &defaults, defaults_file_path, &sd_bytes_written, &default_data_crced, status, reboot_counter);
			}
		} else if (STATUS_IN_ROCKET == status) {
			photores_rckt_lux = photores_get_data(photores_rckt);
			if (photores_rckt_lux > (1 - PHOTORESISTOR_CRITICAL_MODIFICATOR) * ground_lux_rckt) {
				time_out = HAL_GetTick();
				status = STATUS_OUT_OF_ROCKET;
				update_status_in_defaults(&file_system, &defaults, defaults_file_path, &sd_bytes_written, &default_data_crced, status, reboot_counter);
			}
		} else if (STATUS_OUT_OF_ROCKET == status) {
			if (HAL_GetTick() - time_out > STABILISATION_DELAY) {
				status = STATUS_STABILISED;
				update_status_in_defaults(&file_system, &defaults, defaults_file_path, &sd_bytes_written, &default_data_crced, status, reboot_counter);
			}
		} else if (STATUS_STABILISED == status) {
			HAL_GPIO_WritePin(Incinerator_GPIO_Port, Incinerator_Pin, 1);
			time_burning = HAL_GetTick();
			status = STATUS_STARTED_BURNING;
			update_status_in_defaults(&file_system, &defaults, defaults_file_path, &sd_bytes_written, &default_data_crced, status, reboot_counter);
		} else if (STATUS_STARTED_BURNING == status) {
			if (HAL_GetTick() - time_burning > INCINERATOR_DELAY) {
				HAL_GPIO_WritePin(Incinerator_GPIO_Port, Incinerator_Pin, 0);
				status = STATUS_STRING_BURNT;
				update_status_in_defaults(&file_system, &defaults, defaults_file_path, &sd_bytes_written, &default_data_crced, status, reboot_counter);
			}
		} else if (STATUS_STRING_BURNT == status) {
			bmp_data = bmp280_get_data(&bmp280);
			time_height = HAL_GetTick();
			height = calculate_height(bmp_data.pressure, ground_pressure);
			if (prev_height - height < 0.001 * (time_height - time_prev_height)) {
				height_delta_counter++;
				if (height_delta_counter > 9) {
					status = STATUS_LANDED;
					update_status_in_defaults(&file_system, &defaults, defaults_file_path, &sd_bytes_written, &default_data_crced, status, reboot_counter);
				}
			}
			prev_height = height;
			time_prev_height = time_height;
		} else if (STATUS_LANDED == status) {
			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, 1);
			status = STATUS_AFTER;
			update_status_in_defaults(&file_system, &defaults, defaults_file_path, &sd_bytes_written, &default_data_crced, STATUS_LANDED, reboot_counter);
		}
	}
}
