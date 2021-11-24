# ThunderClick
AS3935 lightning sensor drivers and demo

PROJECT: AS3935 Lightning Sensor 

MCU: STM32F407VGT

DEV. BOARD: STM32F407-DISC1

SENSOR: AS3935

IDE: STM32CubeIDE 

AUTHOR: Mario Oletić



PROJECT DESCRIPTION:

In this project, drivers for the AS3935 lightning sensor are developed. This sensor can detect the presence and proximity of potentially hazardous lightning activity in the vicinity and provides estimated distance to the centre of the storm. Developed drivers enable communication and sensor setup, as well as processing and sending received data. Whenever events happen, the AS3935 pulls the IRQ pin high and displays the interrupt in its internal register. The low to high transition on the IRQ pin triggers an external interrupt and MCU then starts communication with the sensor to determine the reason for the interrupt. If the interrupt is caused by the lightning event, the estimated distance to the head of the storm is obtained and sent to PC via UART. Also, the sensor will inform if the interrupt is caused by a high level of noise or by some disturbance.

