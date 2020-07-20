// Include
#include "Interrupts.h"
//
#include <InitConfig.h>
#include "stm32f30x.h"
#include "stdinc.h"
#include "DataTable.h"
#include "Controller.h"
#include "DeviceProfile.h"
#include "LowLevel.h"
#include "Board.h"

// Variables
//
volatile Int64U LED_BlinkTimeCounter = 0;
volatile bool Overflow90 = false;
volatile bool Overflow10 = false;

// Functions
//
void EXTI4_IRQHandler()
{
	if(EXTI_FlagCheck(EXTI_4))
	{
		Overflow90 = true;
		EXTI_FlagReset(EXTI_4);
	}
}
//-----------------------------------------

void EXTI9_5_IRQHandler()
{
	if(EXTI_FlagCheck(EXTI_8))
	{
		Overflow10 = true;
		EXTI_FlagReset(EXTI_8);
	}
}
//-----------------------------------------

void DMA1_Channel1_IRQHandler()
{
	if(DMA_IsTransferComplete(DMA1, DMA_ISR_TCIF1))
	{
		TIM_Stop(TIM6);
		DMA_TransferCompleteReset(DMA1, DMA_IFCR_CTCIF1);
	}
}
//-----------------------------------------

void USART1_IRQHandler()
{
	if(ZwSCI_RecieveCheck(USART1))
	{
		ZwSCI_RegisterToFIFO(USART1);
		ZwSCI_RecieveFlagClear(USART1);
	}
}
//-----------------------------------------

void USB_LP_CAN_RX0_IRQHandler()
{
	if(NCAN_RecieveCheck())
	{
		NCAN_RecieveData();
		NCAN_RecieveFlagReset();
	}
}
//-----------------------------------------

void TIM3_IRQHandler()
{
	if(TIM_StatusCheck(TIM3))
	{
		CONTROL_TimeCounter++;
		
		if(CONTROL_TimeCounter > (LED_BlinkTimeCounter + LED_BLINK_PERIOD))
		{
			LL_ToggleLED();
			LED_BlinkTimeCounter = CONTROL_TimeCounter;
		}
		TIM_StatusClear(TIM3);
	}
}
//-----------------------------------------
