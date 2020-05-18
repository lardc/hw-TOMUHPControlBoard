#ifndef __INITCONFIG_H
#define __INITCONFIG_H

#include "Board.h"

//Functions
//
Boolean SysClk_Config();
void EI_Config();
void IO_Config();
void CAN_Config();
void UART_Config();
void ADC_Init();
void Timer3_Config();
void WatchDog_Config();
void InitializeController(Boolean GoodClock);

#endif //__INITCONFIG_H
