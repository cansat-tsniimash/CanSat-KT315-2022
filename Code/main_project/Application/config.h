#ifndef CONFIG_H_
#define CONFIG_H_

#define shift_reg_imu_conf &hspi2, GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_13, 0
#define shift_reg_rf_conf &hspi2, GPIOC, GPIO_PIN_4, GPIOC, GPIO_PIN_5, 0
#define bmp280_conf 2, &hspi2, &shift_reg_imu
#define lis_conf 3, &hspi2, &shift_reg_imu
#define lsm_conf 4, &hspi2, &shift_reg_imu
#define ds18_conf GPIOA, GPIO_PIN_1

#endif /* CONFIG_H_ */
