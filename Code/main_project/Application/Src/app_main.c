#include <app_main.h>
#include <includes.h>
#include <timers.h>

#include <shift_regs.h>

#include <bmp280.h>
#include <lis3mdl.h>
#include <lsm6ds3.h>
#include <ds18b20.h>
#include <photores.h>
#include <gps.h>

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

	//SHIF_REG
	shift_reg_t shift_reg_imu = shift_reg_create_descriptor(&hspi2, GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_13, 0);
	shift_reg_init_stm32(&shift_reg_imu);

	shift_reg_t shift_reg_rf = shift_reg_create_descriptor(&hspi2, GPIOC, GPIO_PIN_4, GPIOC, GPIO_PIN_5, 0);
	shift_reg_init_stm32(&shift_reg_rf);

	//BMP280
	bme_spi_intf_sr bmp_setup = bmp280_create_descriptor(2, &hspi2, &shift_reg_imu);
	struct bme280_dev bmp280 = bmp280_init(&bmp_setup);

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

	//NRF24
	nrf24_lower_api_config_t nrf24_config = {0};
	nrf24_fifo_status_t rf_fifo_status_rx;
	nrf24_fifo_status_t rf_fifo_status_tx;

	nrf24_spi_pins_sr_t nrf24_sr_setup = nrf24_create_sr_descriptor(&shift_reg_rf, 0, 1);
	nrf24_rf_config_t nrf24_rf_setup = nrf24_create_rf_descriptor(NRF24_DATARATE_250_KBIT,NRF24_TXPOWER_MINUS_0_DBM, 115);
	nrf24_protocol_config_t nrf24_protocol_setup = nrf24_create_protocol_descriptor(NRF24_CRCSIZE_DISABLE, NRF24_ADDRES_WIDTH_5_BYTES, true, true, true, 0, 0);
	nrf24_pipe_config_t nrf24_pipe_setup = nrf24_create_pipe_descriptor(false, 0xacacacacac, -1);
	nrf24_init_stm32(&nrf24_config, &hspi2, &nrf24_sr_setup, &nrf24_rf_setup, &nrf24_protocol_setup, &nrf24_pipe_setup);

	//SD-Card
	FATFS file_system;
	FIL dosimeter_file;
	FIL bmp_file;
	FIL ds_file;
	FIL gps_file;
	FIL inertial_file;
	FIL sebastian_file;
	FRESULT fres = -1;
	char sd_buffer[300] = {0};
	uint16_t sd_buffer_size = 0;
	UINT sd_bytes_written = 0;
	if (f_mount(&file_system, "", 1) == FR_OK) {
		const char dosimeter_file_path[] = "dosimeter.csv";
		const char bmp_file_path[] = "bmp.csv";
		const char ds_file_path[] = "ds.csv";
		const char gps_file_path[] = "gps.csv";
		const char inertial_file_path[] = "inertial.csv";
		const char sebastian_file_path[] = "sebastian.csv";

		fres = f_open(&dosimeter_file, &dosimeter_file_path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&dosimeter_file, "flag;num;time from start;tps;ticks sum;crc\n");
		f_sync(&dosimeter_file);

		fres = f_open(&bmp_file, &bmp_file_path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&bmp_file, "flag;num;time from start;bmp temperature;bmp pressure;crc\n");
		f_sync(&bmp_file);

		fres = f_open(&ds_file, &ds_file_path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&ds_file, "flag;num;time from start; ;crc\n");
		f_sync(&ds_file);

		fres = f_open(&gps_file, &gps_file_path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&gps_file, "flag;num;time from start; ;crc\n");
		f_sync(&gps_file);

		fres = f_open(&inertial_file, &inertial_file_path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&inertial_file, "flag;num;time from start; ;crc\n");
		f_sync(&inertial_file);

		fres = f_open(&sebastian_file, &sebastian_file_path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&sebastian_file, "flag;num;time from start; ;crc\n");
		f_sync(&sebastian_file);
	}
	/* End Init */


	/* Begin data structures */

	bmp_data_t bmp_data = {0};

	lis_data_t lis_data = {0};

	lsm_data_t lsm_data = {0};

	uint16_t ds_temperature = 0;

	float photores_rckt_lux = 0.0;
	float photores_seed_lux = 0.0;

	/* End data structures */


	// Eternal loop
	while(true) {

		// Work~~ OwO

		//Dosimeter
		uint32_t dosimeter_tps = Dosimeter_Get_TPS();
		uint32_t dosimeter_sum = Dosimeter_Get_Sum();
		rf_dosimeter_package_crc_t dosimeter_package = pack_rf_dosimeter(dosimeter_tps, dosimeter_sum);
		send_rf_package(&nrf24_config, &dosimeter_package, sizeof(dosimeter_package));
		sd_buffer_size = sd_parse_to_text_dosimeter(&sd_buffer, dosimeter_package);
		fres = f_write(&dosimeter_file, sd_buffer, sd_buffer_size, &sd_bytes_written);
		fres = f_sync(&dosimeter_file);

		//BMP
		bmp_data = bmp280_get_data(&bmp280);
		rf_bmp_package_crc_t bmp_package = pack_rf_bmp(bmp_data.temperature, bmp_data.pressure);
		send_rf_package(&nrf24_config, &bmp_package, sizeof(bmp_package));
		sd_buffer_size = sd_parse_to_text_bmp(&sd_buffer, bmp_package);
		fres = f_write(&bmp_file, sd_buffer, sd_buffer_size, &sd_bytes_written);
		fres = f_sync(&bmp_file);


		/* Begin working with GNSS */

		//Working with Raw Data buffer
		//size_t tail = 0;
		//size_t head = sizeof(gps_cycle_buffer) - LL_DMA_GetDataLength(DMA2, DMA2_Stream1);
		//while (tail != head) {
		//	gps_push_byte(gps_cycle_buffer[tail]);
		//	tail++;
		//	if (tail >= sizeof(gps_cycle_buffer)) {
		//		tail = 0;
		//	}
		//}
		//Parsing Raw Data
		//gps_work();
		//
		//gps_get_gga(cookie, gga_);

		/* End working with GNSS */

	}
}
