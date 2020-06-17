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
volatile bool Overflow90 = FALSE;
volatile bool Overflow10 = FALSE;

// Functions
//
void EXTI4_IRQHandler()
{
	Overflow90 = TRUE;
	EXTI_FlagReset(EXTI_4);
}
//-----------------------------------------

void EXTI9_5_IRQHandler()
{
	if (EXTI_FlagCheck(EXTI_8))
	{
		Overflow10 = TRUE;
		EXTI_FlagReset(EXTI_8);
	}
}
//-----------------------------------------

void USART1_IRQHandler()
{
	if (ZwSCI_RecieveCheck(USART1))
	{
		ZwSCI_RegisterToFIFO(USART1);
		ZwSCI_RecieveFlagClear(USART1);
	}
}
//-----------------------------------------

void USB_LP_CAN_RX0_IRQHandler()
{
	if (NCAN_RecieveCheck())
	{
		NCAN_RecieveData();
		NCAN_RecieveFlagReset();
	}
}
//-----------------------------------------

void TIM3_IRQHandler()
{
	if (TIM_StatusCheck(TIM3))
	{
		CONTROL_TimeCounter++;

		if (CONTROL_TimeCounter > (LED_BlinkTimeCounter + LED_BLINK_PERIOD))
		{
			LL_ToggleLED();
			LED_BlinkTimeCounter = CONTROL_TimeCounter;
		}
		TIM_StatusClear(TIM3);
	}
}
//-----------------------------------------
