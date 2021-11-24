

/*********************************INCLUDES******************************************/
#include "bsp/AS3935.h"
#include <string.h>
/**********************GLOBAL VARIABLES AND CONSTANTS*******************************/

/* AS3935 Register List */
uint8_t REG00 = 0x00;
uint8_t REG01 = 0x01;
uint8_t REG02 = 0x02;
uint8_t REG03 = 0x03;
uint8_t REG04 = 0x04;
uint8_t REG05 = 0x05;
uint8_t REG06 = 0x06;
uint8_t REG07 = 0x07;
uint8_t REG08 = 0x08;
uint8_t REG09 = 0x3A;
uint8_t REG10 = 0x3B;

/****************************FORWARD DECLARATIONS***********************************/


/*********************************FUNCTIONS*****************************************/

/*   @brief  Function converts write register address of AS3935 to read register address
 *   @param  AS3935 Write register address
 *   @retval AS3935 Read register address
 * */
static uint8_t Read_Address(uint8_t addr)
{
	uint8_t out = 0;
	out = addr | 0x40;

	return out;
}

/*   @brief  Basic AS3935 Read Function
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  AS3935 Register address
 *   @retval Data from specified AS3935 register
 * */
static uint8_t AS3935_Read(SPI_HandleTypeDef *device, uint8_t reg_addr)
{
	uint8_t read_addr = 0;
	uint8_t received_data = 0;

	read_addr = Read_Address(reg_addr);

	HAL_GPIO_WritePin(AS3935_CHIP_SELECT_PORT, AS3935_CHIP_SELECT_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(device, &read_addr, 1, 20);
	HAL_SPI_Receive(device, &received_data, 1, 20);
	HAL_GPIO_WritePin(AS3935_CHIP_SELECT_PORT, AS3935_CHIP_SELECT_PIN, GPIO_PIN_SET);

	return received_data;
}

/*   @brief  Basic AS3935 Write Function
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  AS3935 write register address
 *   @param  Command (or data) to write in the specified register
 * */
void AS3935_Write(SPI_HandleTypeDef *device, uint8_t reg_addr, uint8_t command)
{
	HAL_GPIO_WritePin(AS3935_CHIP_SELECT_PORT, AS3935_CHIP_SELECT_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(device, &reg_addr, 1, 20);
	HAL_SPI_Transmit(device, &command, 1, 20);
	HAL_GPIO_WritePin(AS3935_CHIP_SELECT_PORT, AS3935_CHIP_SELECT_PIN, GPIO_PIN_SET);
}

/*   @brief  Function sets Watch Dog Threshold
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  Watch Dog Thresholds options
 * */
void Set_WatchDogThreshold(SPI_HandleTypeDef *device, wdt_thresh_t factor)
{
   uint8_t val;

   val = AS3935_Read(device, REG01);
   val &= 0xF0;
   val |= factor;
   AS3935_Write(device, REG01, val);
}

/*   @brief  Function sets Noise Floor Threshold for issuing INT_NH interrupt
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  Noise Floor Thresholds options
 * */
void Set_NoiseFloorLevel(SPI_HandleTypeDef *device, uint8_t factor)
{
   uint8_t val;

   val = AS3935_Read(device, REG01);
   val &= 0x8F;
   val |= factor << 4;
   AS3935_Write(device, REG01, val);
}

/*   @brief  Function sets Spike Rejection level for adjusting robustness against false alarms
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  Spike rejection options
 * */
void Set_SpikeRejection(SPI_HandleTypeDef *device, srej_t factor)
{
   uint8_t val;

   val = AS3935_Read(device, REG02);
   val &= 0xF0;
   val |= factor;
   AS3935_Write(device, REG02, val);
}

/*   @brief  Function sets Analog Front-end (AFE) gain
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  Analog Front End Gain options
 *   @note   Use AFE_INDOOR or AFE_OUTDOOR for function arguments since these values are optimized for indoor
 *           and outdoor operating environments
 * */
void Set_AFEGainBoost(SPI_HandleTypeDef *device, afg_t factor)
{
   uint8_t val;

   val = AS3935_Read(device ,REG01);
   val &= 0xC1;
   val |= factor << 1;
   AS3935_Write(device, REG01, val);
}

/*   @brief  Function sets minimum number of lightnings for issuing interrupt
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  Minimum number of Lightning detection options
 * */
void Set_MinimumNumberOfLightning(SPI_HandleTypeDef *device, nol_t factor)
{
   uint8_t val;

   val = AS3935_Read(device, REG02);
   val &= 0xCF;
   val |= factor << 4;
   AS3935_Write(device, REG02, val);
}

/*   @brief  Function sets frequency division ratio for the antenna tuning
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  Frequency division ratio for the antenna tuning options
 * */
void Set_FreqDivision(SPI_HandleTypeDef *device, atun_t factor)
{
   uint8_t val;

   val = AS3935_Read(device, REG03);
   val &= 0x3F;
   val |= factor << 6;
   AS3935_Write(device, REG03, val);
}

/*   @brief  Function sets AS3935 operating modes
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  Power mode type options
 * */
void PowerDownMode(SPI_HandleTypeDef *device, pwr_mode_t factor)
{
   uint8_t rec;

   rec = AS3935_Read(device, REG00);
   rec &= 0xFE;
   rec |= factor;

   AS3935_Write(device, REG00, rec);
}

/*   @brief  Function reads interrupt status register
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @retval Data from AS3935 interrupt register
 *   @note   With this function reason for the interrupt can be determined
 * */
uint8_t AS3935_ReadIntStat(SPI_HandleTypeDef *device)
{
   uint8_t rec;

   rec = AS3935_Read(device, REG03);
   rec &= 0x0F;

   return rec;
}

/*   @brief  Function returns estimated distance to the head of the storm
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @retval Data from AS3935 REG07 register which contains the estimated distance to the head of the approaching storm
 * */
uint8_t Get_Distance(SPI_HandleTypeDef *device)
{
   uint8_t rec;

   rec = AS3935_Read(device, REG07);
   rec &= 0x3F;

   return rec;
}

/*   @brief  Function where basics AS3935 settings are set
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module
 * */
void AS3935_Init(SPI_HandleTypeDef *device)
{

   HAL_GPIO_WritePin(AS3935_CHIP_SELECT_PORT, AS3935_CHIP_SELECT_PIN, GPIO_PIN_SET);

   PowerDownMode(device, PDWN_OFF);
   //Set_SpikeRejection(device, SREJ05);
   Set_WatchDogThreshold(device, WDT_THRES10);
}

/*   @brief  Function checks for events, formats the obtained data and sends info via UART according to events
 *   @param  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 *   @param  huart Pointer to a UART_HandleTypeDef structure that contains the configuration information for the
 *   	     specified UART module.
 * */
void AS3935_To_UART(SPI_HandleTypeDef *device, UART_HandleTypeDef *port)
{
	uint8_t int_status = 0;
	uint8_t est_distance = 0;
	char data_to_send[60] = {0};

	int_status = AS3935_ReadIntStat(device);

	/****************************** Check what caused interrupt **********************************/
	/* If interrupt occurred due to the High Noise Level */
	if((int_status & 0x01) == HIGH_NOISE_LEVEL_INTERRUPT)
	{
		sprintf(data_to_send,"Noise Level To High! Change the noise floor threshold. \r\n");
		HAL_UART_Transmit(port, (uint8_t *)data_to_send, strlen(data_to_send), 50);
	}
	/* If the disturber triggered the interrupt */
	if((int_status & 0x04) == DISTURBER_INTERRUPT)
	{
		sprintf(data_to_send, "Disturber detected!!! \r\n");
		HAL_UART_Transmit(port, (uint8_t *)data_to_send, strlen(data_to_send), 50);
	}
	/* If the lightning is detected*/
	if((int_status & 0x08) == LIGHTNING_INTERRUPT)
	{
		est_distance = Get_Distance(device);

		if(est_distance == 0x3F)
		{
			sprintf(data_to_send, "The storm is out of range!!! \r\n");
			HAL_UART_Transmit(port, (uint8_t *)data_to_send, strlen(data_to_send), 50);
		}
		else if(est_distance = 0x01)
		{
			sprintf(data_to_send, "The storm is overhead!!! \r\n");
			HAL_UART_Transmit(port, (uint8_t *)data_to_send, strlen(data_to_send), 50);
		}
		else
		{
			sprintf(data_to_send, "Estimated storm range: %d km \r\n", est_distance);
			HAL_UART_Transmit(port, (uint8_t *)data_to_send, strlen(data_to_send), 50);
		}
	}
}
