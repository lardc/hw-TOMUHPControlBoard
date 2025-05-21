// Include
//
#include "Global.h"
#include "Controller.h"
#include "Interrupts.h"
#include "SysConfig.h"
#include "BoardConfig.h"
#include "BCCIxParams.h"
#include "Delay.h"

// Defines
#define DAC_RESOLUTION		4095
#define SPI2_BAUDRATE_BITS	0x5
#define DAC_CHANNEL_B		BIT15

// Forward functions
//
void SysClk_Config();
void IO_Config();
void CAN_Config();
void UART_Config();
void Timer2_Config();
void WatchDog_Config();
void SPI2_Config();


// Functions
//
int main()
{
	// Set request flag if firmware update is required
	if (*ProgramAddressStart == 0xFFFFFFFF || BOOT_LOADER_VARIABLE == BOOT_LOADER_REQUEST)
		WaitForFWUpload = TRUE;

	// Init peripherals
	SysClk_Config();
	IO_Config();
	CAN_Config();
	UART_Config();
	SPI2_Config();
	Timer2_Config();
	WatchDog_Config();

	// Init controller
	CONTROL_Init();

	// Infinite cycle
	while (true)
		CONTROL_Idle();

	return 0;
}
//--------------------------------------------

void SysClk_Config()
{
	RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//--------------------------------------------

void IO_Config()
{
	// Включение тактирования портов
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);
	RCC_GPIO_Clk_EN(PORTC);

	// Выходы
	GPIO_Config(GPIOC, Pin_13, Output, PushPull, HighSpeed, NoPull);	// PC13	(LED)
	GPIO_Config(GPIOC, Pin_14, Output, PushPull, HighSpeed, NoPull);	// PC14	(Sync_GD)
	GPIO_Config(GPIOB, Pin_12, Output, PushPull, HighSpeed, NoPull);	// PB12	(CS_GD1)
	GPIO_Config(GPIOB, Pin_14, Output, PushPull, HighSpeed, NoPull);	// PB14	(LDAC)
	GPIO_Config(GPIOB, Pin_2,  Output, PushPull, HighSpeed, NoPull);	// PB2	(SNC_TOCU)
	GPIO_Config(GPIOA, Pin_8,  Output, PushPull, HighSpeed, NoPull);	// PA8	(SYNC)

	GPIO_Bit_Rst(GPIOB, Pin_2);
	GPIO_Bit_Rst(GPIOA, Pin_8);
	GPIO_Bit_Rst(GPIOC, Pin_14);
	GPIO_Bit_Set(GPIOB, Pin_12);
	GPIO_Bit_Set(GPIOB, Pin_14);

	// Альтернативные функции портов
	GPIO_Config(GPIOA, Pin_9, AltFn, PushPull, HighSpeed, NoPull);		// PA9(USART1 TX)
	GPIO_AltFn(GPIOA, Pin_9, AltFn_7);

	GPIO_Config(GPIOA, Pin_10, AltFn, PushPull, HighSpeed, NoPull);		// PA10(USART1 RX)
	GPIO_AltFn(GPIOA, Pin_10, AltFn_7);

	GPIO_Config(GPIOA, Pin_11, AltFn, PushPull, HighSpeed, NoPull);		// PA11(CAN RX)
	GPIO_AltFn(GPIOA, Pin_11, AltFn_9);

	GPIO_Config(GPIOA, Pin_12, AltFn, PushPull, HighSpeed, NoPull);		// PA12(CAN TX)
	GPIO_AltFn(GPIOA, Pin_12, AltFn_9);

	GPIO_Config(GPIOB, Pin_15, AltFn, PushPull, HighSpeed, NoPull);		// PB15(DOUT)
	GPIO_AltFn(GPIOB, Pin_15, AltFn_5);

	GPIO_Config(GPIOB, Pin_13, AltFn, PushPull, HighSpeed, NoPull);		// PB13(SCK)
	GPIO_AltFn(GPIOB, Pin_13, AltFn_5);
}
//--------------------------------------------

void SPI2_Config()
{
	SPI_Init(SPI2, SPI2_BAUDRATE_BITS, false);
	SPI_SetSyncPolarity(SPI2, RISE_Edge);
	SPI_WriteByte(SPI2, 0);

	// Уровень срабатывания компаратора тока управления выставляем высоким,
	// чтобы исключить ложные срабатывания при запуске блока
	GPIO_Bit_Rst(GPIOB, Pin_12);
	SPI_WriteByte(SPI2, DAC_RESOLUTION | DAC_CHANNEL_B);
	GPIO_Bit_Set(GPIOB, Pin_12);
	DELAY_US(1);
	GPIO_Bit_Rst(GPIOB, Pin_14);
	DELAY_US(5);
	GPIO_Bit_Set(GPIOB, Pin_14);
}

void CAN_Config()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, FALSE);
	NCAN_FIFOInterrupt(TRUE);
	NCAN_FilterInit(0, CAN_SLAVE_FILTER_ID, CAN_SLAVE_NID_MASK);
}
//--------------------------------------------

void UART_Config()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 0, true);
}
//--------------------------------------------

void Timer2_Config()
{
	TIM_Clock_En(TIM_2);
	TIM_Config(TIM2, SYSCLK, TIMER2_uS);
	TIM_Interupt(TIM2, 0, true);
	TIM_Start(TIM2);
}
//--------------------------------------------

void WatchDog_Config()
{
	IWDG_Config();
	IWDG_ConfigureSlowUpdate();
}
//--------------------------------------------
