#ifndef SHIFT_REGS_H_
#define SHIFT_REGS_H_

#include <includes.h>

shift_reg_t shift_reg_create_descriptor(SPI_HandleTypeDef *bus, GPIO_TypeDef *latch_port, uint16_t latch_pin, GPIO_TypeDef *oe_port, uint16_t oe_pin, uint16_t value);
void shift_reg_init_stm32(shift_reg_t *shift_reg_descriptor);


#endif /* SHIFT_REGS_H_ */
