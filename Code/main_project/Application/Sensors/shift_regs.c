#include "shift_regs.h"
#include "includes.h"

shift_reg_t shift_reg_create_descriptor(SPI_HandleTypeDef *bus, GPIO_TypeDef *latch_port, uint16_t latch_pin, GPIO_TypeDef *oe_port, uint16_t oe_pin, uint16_t value) {
	shift_reg_t shift_reg_ = {
		.bus = bus,
		.latch_port = latch_port,
		.latch_pin = latch_pin,
		.oe_port = oe_port,
		.oe_pin = oe_pin,
		.value = 0
	};
	return shift_reg_;
}

void shift_reg_init_stm32(shift_reg_t *shift_reg_descriptor) {
	shift_reg_init(shift_reg_descriptor);
	shift_reg_oe(shift_reg_descriptor, true);
	shift_reg_write_8(shift_reg_descriptor, 0xFF);
	shift_reg_oe(shift_reg_descriptor, false);
}
