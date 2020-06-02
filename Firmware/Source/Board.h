#ifndef __BOARD_H
#define __BOARD_H

#include "stm32f30x.h"

#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwTIM.h"
#include "ZwDAC.h"
#include "ZwDMA.h"
#include "ZwADC.h"
#include "ZwEXTI.h"
#include "ZwSCI.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"
#include "ZwSPI.h"

// Аналоговые входы
GPIO_PortPinSettingMacro GPIO_ANLG_I_DUT	= {GPIOA, Pin_1};

// Определения для выходных портов
GPIO_PortPinSettingMacro GPIO_FAN			= {GPIOA, Pin_0};
GPIO_PortPinSettingMacro GPIO_M_RESET		= {GPIOA, Pin_2};
GPIO_PortPinSettingMacro GPIO_LOAD			= {GPIOA, Pin_3};
GPIO_PortPinSettingMacro GPIO_CS			= {GPIOA, Pin_4};
GPIO_PortPinSettingMacro GPIO_IND			= {GPIOA, Pin_7};
GPIO_PortPinSettingMacro GPIO_SYNC			= {GPIOA, Pin_8};
GPIO_PortPinSettingMacro GPIO_SFTY_EN		= {GPIOA, Pin_15};
GPIO_PortPinSettingMacro GPIO_RLC			= {GPIOB, Pin_0};
GPIO_PortPinSettingMacro GPIO_RELAY			= {GPIOB, Pin_1};
GPIO_PortPinSettingMacro GPIO_SNC_TOCU		= {GPIOB, Pin_2};
GPIO_PortPinSettingMacro GPIO_SREG_CS		= {GPIOB, Pin_5};
GPIO_PortPinSettingMacro GPIO_CS_GD2		= {GPIOB, Pin_6};
GPIO_PortPinSettingMacro GPIO_TRIG_RST		= {GPIOB, Pin_7};
GPIO_PortPinSettingMacro GPIO_CS_DAC		= {GPIOB, Pin_9};
GPIO_PortPinSettingMacro GPIO_SREG_OE		= {GPIOB, Pin_11};
GPIO_PortPinSettingMacro GPIO_CS_GD1		= {GPIOB, Pin_12};
GPIO_PortPinSettingMacro GPIO_LDAC			= {GPIOB, Pin_14};
GPIO_PortPinSettingMacro GPIO_LED			= {GPIOC, Pin_13};
GPIO_PortPinSettingMacro GPIO_SYNC_GD		= {GPIOC, Pin_14};
GPIO_PortPinSettingMacro GPIO_PS_EN			= {GPIOC, Pin_15};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX	= {GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX	= {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART_RX	= {GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART_TX	= {GPIOA, Pin_9};
GPIO_PortPinSettingMacro GPIO_ALT_SPI1_CLK	= {GPIOA, Pin_5};
GPIO_PortPinSettingMacro GPIO_ALT_SPI1_MISO	= {GPIOA, Pin_6};
GPIO_PortPinSettingMacro GPIO_ALT_SPI2_CLK	= {GPIOB, Pin_13};
GPIO_PortPinSettingMacro GPIO_ALT_SPI2_MOSI	= {GPIOB, Pin_15};

// Определения для входных портов
GPIO_PortPinSettingMacro GPIO_SFTY			= {GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_OVERFLOW90	= {GPIOB, Pin_4};
GPIO_PortPinSettingMacro GPIO_OVERFLOW10	= {GPIOB, Pin_8};
GPIO_PortPinSettingMacro GPIO_PRESSURE		= {GPIOB, Pin_10};

#endif // __BOARD_H
