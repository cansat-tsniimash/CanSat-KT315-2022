#include <gps.h>
#include <includes.h>

static uint8_t gps_cycle_buffer[500];

void gps_init_stm32() {
	LL_DMA_ConfigAddresses(DMA2, DMA2_Stream1, USART6->DR, gps_cycle_buffer, DMA_PERIPH_TO_MEMORY);
	LL_DMA_SetDataLength(DMA2, DMA2_Stream1, sizeof(gps_cycle_buffer));
	LL_DMA_EnableStream(DMA2, DMA2_Stream1);
	LL_USART_EnableDMAReq_RX(USART6);
	gps_init();
}

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
