#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#include "fatfs.h"
#include "LSM6DS3/DLSM.h"
#include "BME280/DriverForBME280.h"
#include "nRF24L01_PL/nrf24_defs.h"
#include "nRF24L01_PL/nrf24_upper_api.h"
#include "nRF24L01_PL/nrf24_lower_api_stm32.h"
#include "nRF24L01_PL/nrf24_lower_api.h"
#include "Photorezistor/photorezistor.h"



extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

//радио: настройка радио-части(+) и настройка протокольной части(+), настройка пайпа(+), перегон в режим отправки (тх) (+); потом радио пакты пихаем в ФИФО(проверка на свободное место в ФИФО),
unsigned short Crc16(unsigned char *buf, unsigned short len)
{
	unsigned short crc = 0xFFFF;//переменная 16 бит = 2 байта
	unsigned char i; //переменная 8 бит = 1 байт
	while (len--)// проверка условия продолжения
	{
		crc ^= *buf++ << 8;
		for (i = 0; i < 8; i++)//цикл перебора полинома
			crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
	}
	return crc;//конец функции расчёта Crc16
}

typedef struct reg_param_t
{
	uint8_t addr;
	const char * name;
	uint8_t size;
} reg_param_t;

#pragma pack(push,1)
// Структура пакетов в радио
typedef struct
{
	uint8_t flag;

	int16_t BME280_temperature;
	int16_t LSM6DSL_accelerometr_x;
	int16_t LSM6DSL_accelerometr_y;
	int16_t LSM6DSL_accelerometr_z;
	int16_t LSM6DSL_gyroscope_x;
	int16_t LSM6DSL_gyroscope_y;
	int16_t LSM6DSL_gyroscope_z;
	uint16_t num;
	uint16_t crc;

	uint32_t time;
	uint32_t BME280_pressure;
} packet_da_type_1_t;
#pragma pack(pop)

int app_main(void)
{

    // Карта памяти (ПЕРЕМЕННЫЕ)
	FATFS fileSystem; // переменная типа FATFS
	FIL SDFile; // хендлер файла
	UINT CheckBytes; // количество символов, реально записанных внутрь файла
	FRESULT resSD; // результат выполнения функции
	uint8_t path[11] = "SDcard.csv";
	path[10] =  '\0';
	// Переменные для работы с бме и лсм
	struct bme280_dev bme = {0};
	stmdev_ctx_t ctx = {0};


	// НАСТРОЙКА РАДИО (СТРАКТЫ)
	shift_reg_t shift_reg_ = {
		.bus = &hspi2,
		.latch_port= GPIOC,
		.latch_pin = GPIO_PIN_1,
		.oe_port = GPIOC,
		.oe_pin = GPIO_PIN_13,
		.value = 0
	};
	shift_reg_init(&shift_reg_);
	shift_reg_oe(&shift_reg_, true);
	shift_reg_write_8(&shift_reg_, 0xFF);

	//Сдвиговый регистр радио
	shift_reg_t shift_reg_rf = {
		.bus = &hspi2,
		.latch_port = GPIOC,
		.latch_pin = GPIO_PIN_4,
		.oe_port = GPIOC,
		.oe_pin = GPIO_PIN_5,
		.value = 0
	};
	shift_reg_init(&shift_reg_rf);
	shift_reg_oe(&shift_reg_rf, true);
	shift_reg_write_8(&shift_reg_rf, 0xFF);

	// Структура, содержащая параметры SPI пинов  Chip Enab и SPI Chip Select для сдвигового регистра
	nrf24_spi_pins_sr_t nrf24_spi_pins_sr = {
		.pos_CE = 0,
		.pos_CS = 1,
		.this = &shift_reg_rf
	};
	nrf24_lower_api_config_t nrf24_lower_api_config;


	// Настройка радиомодуля
	nrf24_rf_config_t rf_cfg_radio = {
		.data_rate = NRF24_DATARATE_250_KBIT,
		.tx_power = NRF24_TXPOWER_MINUS_0_DBM,
		.rf_channel = 111
	};

	// Настройка протокола
	nrf24_protocol_config_t protocol_cfg_radio = {
		.crc_size = NRF24_CRCSIZE_1BYTE,
		.address_width = NRF24_ADDRES_WIDTH_5_BYTES,
		.en_dyn_payload_size = true,
		.en_ack_payload = true,
		.en_dyn_ack = true,
		.auto_retransmit_count = 0,
		.auto_retransmit_delay = 0
	};
	nrf24_pipe_config_t pipe_config = {
		.enable_auto_ack = true,
		.address = 0xdadadadada,
		.payload_size=-1
	};

	nrf24_spi_init_sr(&nrf24_lower_api_config, &hspi2, &nrf24_spi_pins_sr);
	nrf24_mode_power_down(&nrf24_lower_api_config);
	nrf24_setup_rf(&nrf24_lower_api_config, &rf_cfg_radio);
	nrf24_setup_protocol(&nrf24_lower_api_config, &protocol_cfg_radio);
	nrf24_pipe_rx_start(&nrf24_lower_api_config, 0, &pipe_config);
	nrf24_pipe_rx_start(&nrf24_lower_api_config, 1, &pipe_config);
	nrf24_pipe_set_tx_addr(&nrf24_lower_api_config, 0xacacacacac);

	// НАСТРОЙКА ЛСМ И БМЕ, Фоторезистор (СТРАКТЫ)

	bme_spi_intf_sr spi_interface_bme = {
		.sr_pin = 2,
		.spi = &hspi2,
		.sr = &shift_reg_
	};
	lsm_spi_intf_sr spi_interface_lsm = {
		.sr_pin = 4,
		.spi = &hspi2,
		.sr = &shift_reg_
	};
	photorezistor_t photorez_set = {
		.resist = 5100,
		.hadc = &hadc1
	};

	// ОСНОВНАЯ ЧАСТЬ, САМИ ФУНКЦИИ ИНИЦИАЛИЗАЦИИ ПРИБОРОВ
	resSD = f_mount(&fileSystem, "", 1);
	if(resSD != FR_OK) {
		printf("mount error, %d\n", resSD);
	}
	f_open(&SDFile, (char*)path, FA_WRITE | FA_CREATE_ALWAYS);

	nrf24_mode_standby(&nrf24_lower_api_config);
	//nrf24_mode_tx(&nrf24_lower_api_config);
	//dump_registers(&nrf24_lower_api_config);

	spi_interface_lsm.sr = &shift_reg_;
	spi_interface_bme.sr = &shift_reg_;

	bme_init_default_sr(&bme, &spi_interface_bme);
	lsmset_sr (&ctx, &spi_interface_lsm);
	float acc_g[3];
	float gyro_dps[3];
	float temperature_celsius_mag;
	char headbuffer[1000];
	int headcount = snprintf(headbuffer, 1000, "ax;ay;az;gx;gy;gz;temp;press;lux\n");
	f_write(&SDFile, (uint8_t*) headbuffer, headcount, &CheckBytes);
	f_sync(&SDFile);
	uint16_t packet_num = 0;
	while(1) {
		HAL_ADC_Start(&hadc1);
		nrf24_fifo_status_t Status_FIFO_RX;
		nrf24_fifo_status_t Status_FIFO_TX;
		struct bme280_data comp_data = bme_read_data(&bme);
		float lux = photorezistor_get_lux(photorez_set);
		lsmread(&ctx, &temperature_celsius_mag, &acc_g, &gyro_dps);
		packet_da_type_1_t packet = {0};
		packet.flag = 228;
		packet.BME280_temperature = 10 * comp_data.temperature;
		packet.BME280_pressure = comp_data.pressure;
		packet.LSM6DSL_accelerometr_x = (int16_t)(1000 * acc_g[0]);
		packet.LSM6DSL_accelerometr_y = (int16_t)(1000 * acc_g[1]);
		packet.LSM6DSL_accelerometr_z = (int16_t)(1000 * acc_g[2]);
		packet.LSM6DSL_gyroscope_x = (int16_t)(1000 * gyro_dps[0]);
		packet.LSM6DSL_gyroscope_y = (int16_t)(1000 * gyro_dps[1]);
		packet.LSM6DSL_gyroscope_z = (int16_t)(1000 * gyro_dps[2]);
		packet.num = packet_num++;
		packet.time = HAL_GetTick();
		packet.crc = Crc16((uint8_t*) &packet, sizeof(packet));

		char snbuffer[1000];
		int count = snprintf(snbuffer, 1000, "%10lf;%10lf;%10lf;%10lf;%10lf;%10lf;%lf;%lf;%lf\n", acc_g[0], acc_g[1], acc_g[2], gyro_dps[0], gyro_dps[1], gyro_dps[2], comp_data.pressure, comp_data.temperature, lux);
		f_write(&SDFile, (uint8_t*) snbuffer, count, &CheckBytes);
		f_sync(&SDFile);


		//dump_registers(&nrf24_lower_api_config);
		nrf24_fifo_status(&nrf24_lower_api_config, &Status_FIFO_RX, &Status_FIFO_TX);
		if (Status_FIFO_TX != NRF24_FIFO_FULL) {
			nrf24_fifo_write(&nrf24_lower_api_config, (uint8_t*) &packet , sizeof(packet), false);
			nrf24_mode_tx(&nrf24_lower_api_config);
			HAL_Delay(10);
			nrf24_mode_standby(&nrf24_lower_api_config);
		} else {
			HAL_Delay(100);
			nrf24_fifo_flush_tx(&nrf24_lower_api_config);
		}

		//nrf24_irq_clear(&nrf24_lower_api_config, NRF24_IRQ_RX_DR | NRF24_IRQ_TX_DR | NRF24_IRQ_MAX_RT);
		//HAL_UART_Transmit(&huart1, (uint8_t*) &packet, sizeof(packet), 100);
		printf("lux: %lf\n", lux);
	}

	return 0;
}