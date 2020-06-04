#include "InitConfig.h"

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
	GPIO_InitAnalog(GPIO_ANLG_I_DUT);
	
	// Выходы
	GPIO_InitPushPullOutput(GPIO_FAN);
	GPIO_InitPushPullOutput(GPIO_M_RESET);
	GPIO_InitPushPullOutput(GPIO_LOAD);
	GPIO_InitPushPullOutput(GPIO_CS);
	GPIO_InitPushPullOutput(GPIO_IND);
	GPIO_InitPushPullOutput(GPIO_SYNC);
	GPIO_InitPushPullOutput(GPIO_SFTY_EN);
	GPIO_InitPushPullOutput(GPIO_RLC);
	GPIO_InitPushPullOutput(GPIO_RELAY);
	GPIO_InitPushPullOutput(GPIO_SNC_TOCU);
	GPIO_InitPushPullOutput(GPIO_SREG_CS);
	GPIO_InitPushPullOutput(GPIO_CS_GD2);
	GPIO_InitPushPullOutput(GPIO_CS_DAC);
	GPIO_InitPushPullOutput(GPIO_SREG_OE);
	GPIO_InitPushPullOutput(GPIO_CS_GD1);
	GPIO_InitPushPullOutput(GPIO_LDAC);
	GPIO_InitPushPullOutput(GPIO_LED);
	GPIO_InitPushPullOutput(GPIO_SYNC_GD);
	GPIO_InitPushPullOutput(GPIO_PS_EN);
	//
	GPIO_SetState(GPIO_CS, true);
	GPIO_SetState(GPIO_SREG_CS, true);
	GPIO_SetState(GPIO_CS_GD1, true);
	GPIO_SetState(GPIO_CS_GD2, true);
	GPIO_SetState(GPIO_CS_DAC, true);
	GPIO_SetState(GPIO_LDAC, true);

	// Выход с открытым коллектором
	GPIO_InitOpenDrainOutput(GPIO_TRIG_RST, NoPull);
	GPIO_SetState(GPIO_TRIG_RST, true);
	
	// Входы
	GPIO_InitInput(GPIO_SFTY, NoPull);
	GPIO_InitInput(GPIO_OVERFLOW90, NoPull);
	GPIO_InitInput(GPIO_OVERFLOW10, NoPull);
	GPIO_InitInput(GPIO_PRESSURE, NoPull);
	
	// Альтернативные функции
	GPIO_InitAltFunction(GPIO_ALT_CAN_RX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_CAN_TX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_UART_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART_TX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_SPI1_CLK, AltFn_5);
	GPIO_InitAltFunction(GPIO_ALT_SPI1_MISO, AltFn_5);
	GPIO_InitAltFunction(GPIO_ALT_SPI2_CLK, AltFn_5);
	GPIO_InitAltFunction(GPIO_ALT_SPI2_MOSI, AltFn_5);
}
//------------------------------------------------------------------------------

void CAN_Config()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, FALSE);
	NCAN_FIFOInterrupt(TRUE);
	NCAN_FilterInit(0, 0, 0); // Фильтр 0 пропускает все сообщения
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
	SPI_Init(SPI1, SPI1_BAUDRATE_BITS, SPI1_LSB_FIRST);
	SPI_Init(SPI2, SPI2_BAUDRATE_BITS, SPI2_LSB_FIRST);
}
//------------------------------------------------------------------------------

void WatchDog_Config()
{
	IWDG_Config();
	IWDG_ConfigureFastUpdate();
}
//------------------------------------------------------------------------------
