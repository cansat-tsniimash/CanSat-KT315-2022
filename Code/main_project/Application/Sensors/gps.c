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
