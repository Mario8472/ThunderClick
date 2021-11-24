#ifndef AS3935_H_
#define AS3935_H_

/*********************************INCLUDES******************************************/
#include "stm32f4xx_hal.h"

/**********************************DEFINES******************************************/
#define AS3935_CHIP_SELECT_PORT				GPIOE
#define AS3935_CHIP_SELECT_PIN				GPIO_PIN_5

#define HIGH_NOISE_LEVEL_INTERRUPT			0x01
#define DISTURBER_INTERRUPT					0x04
#define LIGHTNING_INTERRUPT					0x08

/*********************************STRUCTURES****************************************/

/* Power mode type */
typedef enum
{
	PDWN_OFF = 0x00,
	PDWN_ON
}pwr_mode_t;

/* Analog Front End Gain */
typedef enum
{
	AFE_INDOOR = 0x12,  // DEFAULT
	AFE_OUTDOOR = 0x0E
}afg_t;

/* Watch Dog Thresholds  */
typedef enum
{
	WDT_THRES00 = 0x00,
	WDT_THRES01,
	WDT_THRES02,
	WDT_THRES03,
	WDT_THRES04,
	WDT_THRES05,
	WDT_THRES06,
	WDT_THRES07,
	WDT_THRES08,
	WDT_THRES09,
	WDT_THRES10,
	WDT_THRES11,
	WDT_THRES12,
	WDT_THRES13,
	WDT_THRES14,
	WDT_THRES15
}wdt_thresh_t;

/* Noise Floor Thresholds */
typedef enum
{
	NLF_390 = 0x00,
	NLF_630,
	NLF_860,
	NLF_1100,
	NLF_1140,
	NLF_1570,
	NLF_1800,
	NLF_2000
}nlf_t;

/* Minimum number of Lightning detection */
typedef enum
{
	NOL_01 = 0x00,
	NOL_05,
	NOL_09,
	NOL_16
}nol_t;

/* Frequency division ratio for the antenna tuning */
typedef enum
{
	ATUN_16 = 0x00,
	ATUN_32,
	ATUN_64,
	ATUN_128
}atun_t;

/* Spike rejection */
typedef enum
{
	SREJ00 = 0x00,
	SREJ01,
	SREJ02,
	SREJ03,
	SREJ04,
	SREJ05,
	SREJ06,
	SREJ07,
	SREJ08,
	SREJ09,
	SREJ10,
	SREJ11,
	SREJ12,
	SREJ13,
	SREJ14,
	SREJ15
}srej_t;

/******************************EXTERN VARIABLES*************************************/

/****************************FUNCTION PROTOTYPES************************************/
void AS3935_Init(SPI_HandleTypeDef *device);
uint8_t AS3935_ReadIntStat(SPI_HandleTypeDef *device);
uint8_t Get_Distance(SPI_HandleTypeDef *device);
void AS3935_To_UART(SPI_HandleTypeDef *device, UART_HandleTypeDef *port);

#endif /* AS3935_H_ */
