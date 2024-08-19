#ifndef RC522_H
#define RC522_H

#include "stm32f4xx_hal.h"
#include "rc522.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define RC522_RST_PIN GPIO_PIN_8
#define RC522_RST_GPIO_PORT GPIOA
#define RC522_MISO_PIN GPIO_PIN_2
#define RC522_MISO_GPIO_PORT GPIOC
#define RC522_MOSI_PIN GPIO_PIN_1
#define RC522_MOSI_GPIO_PORT GPIOC
#define RC522_SCK_PIN GPIO_PIN_9
#define RC522_SCK_GPIO_PORT GPIOA
#define RC522_NSS_PIN GPIO_PIN_12
#define RC522_NSS_GPIO_PORT GPIOB

void rc522_init(void);
void rc522_reset(void);
void rc522_regWrite8(uint8_t reg, uint8_t data);
uint8_t rc522_regRead8(uint8_t reg);
void rc522_setBit(uint8_t reg, uint8_t mask);
void rc522_clearBit(uint8_t reg, uint8_t mask);
void rc522_antennaON(void);
bool rc522_checkCard(uint8_t *id);
bool rc522_request(uint8_t reqMode, uint8_t *tagType);
bool rc522_antiColl(uint8_t *serNum);
void rc522_halt(void);
void rc522_toCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen);

#endif  // RC522_H
