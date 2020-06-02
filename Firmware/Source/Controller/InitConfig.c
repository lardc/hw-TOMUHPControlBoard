#include <InitConfig.h>


// Forward functions
//
void CONTROL_Init();


// Functions
//
Boolean SysClk_Config()
{
	return RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//------------------------------------------------------------------------------

void EI_Config()
{
	// Overflow 90%
	EXTI_Config(EXTI_PB, EXTI_4, FALL_TRIG, 0);
	EXTI_EnableInterrupt(EXTI4_IRQn, 0, true);

	// Overflow 10%
	EXTI_Config(EXTI_PB, EXTI_8, FALL_TRIG, 0);
	EXTI_EnableInterrupt(EXTI9_5_IRQn, 0, true);
}
//------------------------------------------------------------------------------

void IO_Config()
{
	// Включение тактирования портов
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);
	RCC_GPIO_Clk_EN(PORTC);

	// Аналоговые входы
	GPIO_Config (GPIOA, Pin_1, Analog, NoPull, HighSpeed, NoPull);				// PA1 - I_DUT

	// Выходы
	GPIO_Config (GPIOA, Pin_0,  Output, PushPull, HighSpeed, NoPull);			// PA0 - FAN
	GPIO_Bit_Rst(GPIOA, Pin_0);
	GPIO_Config (GPIOA, Pin_2,  Output, PushPull, HighSpeed, NoPull);			// PA2 - M_RESET
	GPIO_Bit_Rst(GPIOA, Pin_2);
	GPIO_Config (GPIOA, Pin_3,  Output, PushPull, HighSpeed, NoPull);			// PA3 - LOAD
	GPIO_Bit_Rst(GPIOA, Pin_3);
	GPIO_Config (GPIOA, Pin_4,  Output, PushPull, HighSpeed, NoPull);			// PA4 - CS
	GPIO_Bit_Rst(GPIOA, Pin_4);
	GPIO_Config (GPIOA, Pin_7,  Output, PushPull, HighSpeed, NoPull);			// PA7 - IND
	GPIO_Bit_Rst(GPIOA, Pin_7);
	GPIO_Config (GPIOA, Pin_8,  Output, PushPull, HighSpeed, NoPull);			// PA8 - SYNC
	GPIO_Bit_Rst(GPIOA, Pin_8);
	GPIO_Config (GPIOA, Pin_15,  Output, PushPull, HighSpeed, NoPull);			// PA15 - SFTY_EN
	GPIO_Bit_Rst(GPIOA, Pin_15);
	GPIO_Config (GPIOB, Pin_0,  Output, PushPull, HighSpeed, NoPull);			// PB0 - RLC
	GPIO_Bit_Rst(GPIOB, Pin_0);
	GPIO_Config (GPIOB, Pin_1,  Output, PushPull, HighSpeed, NoPull);			// PB1 - RELAY
	GPIO_Bit_Rst(GPIOB, Pin_1);
	GPIO_Config (GPIOB, Pin_2,  Output, PushPull, HighSpeed, NoPull);			// PB2 - SNC_TOCU
	GPIO_Bit_Rst(GPIOB, Pin_2);
	GPIO_Config (GPIOB, Pin_5,  Output, PushPull, HighSpeed, NoPull);			// PB5 - SREG_CS
	GPIO_Bit_Rst(GPIOB, Pin_5);
	GPIO_Config (GPIOB, Pin_6,  Output, PushPull, HighSpeed, NoPull);			// PB6 - CS_GD2
	GPIO_Bit_Rst(GPIOB, Pin_6);
	GPIO_Config (GPIOB, Pin_7,  Output, OpenDrain, HighSpeed, NoPull);			// PB7 - TRIG_RST
	GPIO_Bit_Set(GPIOB, Pin_7);
	GPIO_Config (GPIOB, Pin_9,  Output, PushPull, HighSpeed, NoPull);			// PB9 - CS_DAC
	GPIO_Bit_Rst(GPIOB, Pin_9);
	GPIO_Config (GPIOB, Pin_11,  Output, PushPull, HighSpeed, NoPull);			// PB11 - SREG_OE
	GPIO_Bit_Rst(GPIOB, Pin_11);
	GPIO_Config (GPIOB, Pin_12,  Output, PushPull, HighSpeed, NoPull);			// PB12 - CS_GD1
	GPIO_Bit_Rst(GPIOB, Pin_12);
	GPIO_Config (GPIOB, Pin_14,  Output, PushPull, HighSpeed, NoPull);			// PB14 - LDAC
	GPIO_Bit_Rst(GPIOB, Pin_14);
	GPIO_Config (GPIOC, Pin_13,  Output, PushPull, HighSpeed, NoPull);			// PC13 - LED
	GPIO_Bit_Rst(GPIOC, Pin_13);
	GPIO_Config (GPIOC, Pin_14,  Output, PushPull, HighSpeed, NoPull);			// PC14 - Sync_GD
	GPIO_Bit_Rst(GPIOC, Pin_14);
	GPIO_Config (GPIOC, Pin_15,  Output, PushPull, HighSpeed, NoPull);			// PC15 - PS_EN
	GPIO_Bit_Rst(GPIOC, Pin_15);


	// Входы
	GPIO_Config (GPIOB, Pin_3, Input, NoPull, HighSpeed, NoPull);				// PB3 - SFTY
	GPIO_Config (GPIOB, Pin_4, Input, NoPull, HighSpeed, NoPull);				// PB4 - Overflow90
	GPIO_Config (GPIOB, Pin_8, Input, NoPull, HighSpeed, NoPull);				// PB8 - Overflow10
	GPIO_Config (GPIOB, Pin_10, Input, NoPull, HighSpeed, NoPull);				// PB10 - Pressure


	// Альтернативные функции
	GPIO_Config (GPIOA, Pin_11, AltFn, PushPull, HighSpeed, NoPull);			// PA11 (CAN RX)
	GPIO_AltFn  (GPIOA, Pin_11, AltFn_9);
	GPIO_Config (GPIOA, Pin_12, AltFn, PushPull, HighSpeed, NoPull);			// PA12 (CAN TX)
	GPIO_AltFn  (GPIOA, Pin_12, AltFn_9);
	GPIO_Config (GPIOA, Pin_9, AltFn, PushPull, HighSpeed, NoPull);				// PA9(USART1 TX)
	GPIO_AltFn  (GPIOA, Pin_9, AltFn_7);
	GPIO_Config (GPIOA, Pin_10, AltFn, PushPull, HighSpeed, NoPull);			// PA10(USART1 RX)
	GPIO_AltFn  (GPIOA, Pin_10, AltFn_7);
	GPIO_Config (GPIOA, Pin_5, AltFn, PushPull, HighSpeed, NoPull);				// PA5(SPI1_CLK)
	GPIO_AltFn  (GPIOA, Pin_5, AltFn_5);
	GPIO_Config (GPIOA, Pin_6, AltFn, PushPull, HighSpeed, NoPull);				// PA6(SPI1_MISO)
	GPIO_AltFn  (GPIOA, Pin_6, AltFn_5);
	GPIO_Config (GPIOB, Pin_13, AltFn, PushPull, HighSpeed, NoPull);			// PB13(SPI2_CLK)
	GPIO_AltFn  (GPIOB, Pin_13, AltFn_5);
	GPIO_Config (GPIOB, Pin_15, AltFn, PushPull, HighSpeed, NoPull);			// PB15(SPI2_MOSI)
	GPIO_AltFn  (GPIOB, Pin_15, AltFn_5);
}
//------------------------------------------------------------------------------

void CAN_Config()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, FALSE);
	NCAN_FIFOInterrupt(TRUE);
	NCAN_FilterInit(0, 0, 0);		// Фильтр 0 пропускает все сообщения
}
//------------------------------------------------------------------------------

void UART_Config()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 0, true);
}
//------------------------------------------------------------------------------

void ADC_Init()
{
	RCC_ADC_Clk_EN(ADC_12_ClkEN);
	ADC_Calibration(ADC1);
	ADC_SoftTrigConfig(ADC1);
	ADC_ChannelSet_SampleTime(ADC1, 1, ADC_SMPL_TIME_7_5);
	ADC_ChannelSet_SampleTime(ADC1, 2, ADC_SMPL_TIME_7_5);
	ADC_Enable(ADC1);
}
//------------------------------------------------------------------------------

void Timer3_Config()
{
	TIM_Clock_En(TIM_3);
	TIM_Config(TIM3, SYSCLK, TIMER3_uS);
	TIM_Interupt(TIM3, 0, true);
	TIM_Start(TIM3);
}
//------------------------------------------------------------------------------

void SPI_Config()
{
	SPI_Init(SPI1, 5, false);
	SPI_Init(SPI2, 5, false);
}
//------------------------------------------------------------------------------

void WatchDog_Config()
{
	IWDG_Config();
	IWDG_ConfigureFastUpdate();
}
//------------------------------------------------------------------------------

void InitializeController(Boolean GoodClock)
{
	CONTROL_Init();
}
// -----------------------------------------------------------------------------
