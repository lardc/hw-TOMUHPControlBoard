#include "Board.h"
#include "LowLevel.h"


//Functions
//
void CUSTINT_SendRaw(uint16_t Data);
void CUSTINT_SendTOCU(float Current, bool Fan, bool Contactor, bool PSon);
uint32_t CUSTINT_ReceiveDataSR();
uint32_t CUSTINT_UnpackData10SR(uint32_t Data);
uint32_t CUSTINT_UnpackData90SR(uint32_t Data);
