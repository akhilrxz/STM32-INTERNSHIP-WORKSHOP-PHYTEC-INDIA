#include "rc522.h"
#include "stm32f4xx_hal.h"

// Define the SPI handle
SPI_HandleTypeDef hspi2;

void rc522_init(void) {
    // Initialize SPI2
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi2) != HAL_OK) {
        // Initialization Error
        Error_Handler();
    }

    // Enable GPIO Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Initialize GPIO pins for SPI
    GPIO_InitTypeDef GPIO_InitStruct;

    // Initialize MISO pin
    GPIO_InitStruct.Pin = RC522_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(RC522_MISO_GPIO_PORT, &GPIO_InitStruct);

    // Initialize MOSI pin
    GPIO_InitStruct.Pin = RC522_MOSI_PIN;
    HAL_GPIO_Init(RC522_MOSI_GPIO_PORT, &GPIO_InitStruct);

    // Initialize SCK pin
    GPIO_InitStruct.Pin = RC522_SCK_PIN;
    HAL_GPIO_Init(RC522_SCK_GPIO_PORT, &GPIO_InitStruct);

    // Initialize NSS pin as output
    GPIO_InitStruct.Pin = RC522_NSS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(RC522_NSS_GPIO_PORT, &GPIO_InitStruct);

    // Initialize RST pin as output
    GPIO_InitStruct.Pin = RC522_RST_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(RC522_RST_GPIO_PORT, &GPIO_InitStruct);

    // Reset RC522
    rc522_reset();

    // Initialize RC522
    rc522_regWrite8(MFRC522_REG_T_MODE, 0x80);
    rc522_regWrite8(MFRC522_REG_T_PRESCALER, 0xA9);
    rc522_regWrite8(MFRC522_REG_T_RELOAD_L, 0xE8);
    rc522_regWrite8(MFRC522_REG_T_RELOAD_H, 0x03);
    rc522_regWrite8(MFRC522_REG_TX_AUTO, 0x40);
    rc522_regWrite8(MFRC522_REG_MODE, 0x3D);

    rc522_antennaON();
}

void rc522_reset(void) {
    HAL_GPIO_WritePin(RC522_RST_GPIO_PORT, RC522_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(RC522_RST_GPIO_PORT, RC522_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
}

void rc522_regWrite8(uint8_t reg, uint8_t data) {
    uint8_t txData[2] = {((reg << 1) & 0x7E), data};
    HAL_GPIO_WritePin(RC522_NSS_GPIO_PORT, RC522_NSS_PIN, GPIO_PIN_RESET); // NSS Low
    HAL_SPI_Transmit(&hspi2, txData, 2, 100);
    HAL_GPIO_WritePin(RC522_NSS_GPIO_PORT, RC522_NSS_PIN, GPIO_PIN_SET); // NSS High
}

uint8_t rc522_regRead8(uint8_t reg) {
    uint8_t txData[1] = {((reg << 1) & 0x7E) | 0x80};
    uint8_t rxData[1] = {0};
    HAL_GPIO_WritePin(RC522_NSS_GPIO_PORT, RC522_NSS_PIN, GPIO_PIN_RESET); // NSS Low
    HAL_SPI_TransmitReceive(&hspi2, txData, rxData, 1, 100);
    HAL_GPIO_WritePin(RC522_NSS_GPIO_PORT, RC522_NSS_PIN, GPIO_PIN_SET); // NSS High
    return rxData[0];
}

void rc522_setBit(uint8_t reg, uint8_t mask) {
    rc522_regWrite8(reg, rc522_regRead8(reg) | mask);
}

void rc522_clearBit(uint8_t reg, uint8_t mask) {
    rc522_regWrite8(reg, rc522_regRead8(reg) & (~mask));
}

void rc522_antennaON(void) {
    uint8_t value = rc522_regRead8(MFRC522_REG_TX_CONTROL);
    if ((value & 0x03) != 0x03) {
        rc522_setBit(MFRC522_REG_TX_CONTROL, 0x03);
    }
}

void Error_Handler(void) {
    // Error handling implementation
    while (1) {
        // Stay here in case of error
    }
}
