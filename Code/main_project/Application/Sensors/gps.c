#include <gps.h>
#include <includes.h>

static uint8_t gps_cycle_buffer[500];
static int64_t prev_cookie = 0;
static int64_t cookie_ = 0;
static size_t tail = 0;
static size_t head = 0;

void gps_init_stm32(void) {
	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_1, USART6->DR, (uint32_t)gps_cycle_buffer, DMA_PERIPH_TO_MEMORY);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_1, sizeof(gps_cycle_buffer));
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
	LL_USART_EnableDMAReq_RX(USART6);
	gps_init();
}

void gps_parse_buffer(void) {
	head = sizeof(gps_cycle_buffer) - LL_DMA_GetDataLength(DMA2, LL_DMA_STREAM_1);
	while (tail != head) {
		gps_push_byte(gps_cycle_buffer[tail]);
		tail++;
		if (tail >= sizeof(gps_cycle_buffer)) {
			tail = 0;
		}
	}
}

bool gps_get_data(gps_data_t *gps_data_) {
	gps_parse_buffer();
	gps_work();
	int fix_ = 0;
	gps_get_coords(&cookie_, &gps_data_->latitude, &gps_data_->longtitude, &gps_data_->altitude, &fix_);
	gps_data_->fix = (uint8_t)fix_;
	gps_get_time(&cookie_, &gps_data_->time_sec, &gps_data_->time_microsec);
	if (cookie_ == prev_cookie) {
		return false;
	} else {
		prev_cookie = cookie_;
		return true;
	}
}
