#include "Controller.h"
#include "InitConfig.h"
#include "SysConfig.h"

// Functions
//
void CONTROL_Idle();

int main()
{
	__disable_irq();
	SCB->VTOR = (uint32_t)BOOT_LOADER_MAIN_PR_ADDR;
	__enable_irq();

	// ��������� ��������� ������� ������������
	SysClk_Config();

	// ��������� ������
	IO_Config();

	// ��������� ������� ����������
	EI_Config();

	// ��������� CAN
	CAN_Config();

	// ��������� UART
	UART_Config();

	// SPI config
	SPI_Config();

	// ��������� ���
	ADC_Config();

	// ��������� DMA
	DMA_Config();

	// ��������� ��������
	Timer3_Config();
	Timer6_Config();

	// ��������� ����������� �������
	WatchDog_Config();

	// ������������� ������ �����������
	CONTROL_Init();

	// ������� ����
	while(TRUE)
		CONTROL_Idle();

	return 0;
}
