/*
 * cxx_main.cpp
 *
 *  Created on: 2020年4月20日
 *      Author: HOW
 */

#include "cxx_main.h"


#include "FixBuffer.hpp"
#include "arc_console.hpp"

#include "main.h"

#include "stm32h7xx_hal.h"

// ----------------------------------------------------------
extern UART_HandleTypeDef huart3;
FixBuffer<char,1024> buffer;
#ifdef __cplusplus
extern "C" {
#endif

int _write(int file, char *ptr, int len)
{
	for(int i = 0 ; i < len ; i++)
	{
		buffer.Push(ptr[i]);
	}
	return len;
}
#ifdef __cplusplus
}
#endif
// ----------------------------------------------------------
extern ts_share_infomation info;
// ----------------------------------------------------------
#define WORD_TO_BINARY_PATTERN "%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c"
#define WORD_TO_BINARY(word)  \
  (word & 0x8000 ? '1' : '0'), \
  (word & 0x4000 ? '1' : '0'), \
  (word & 0x2000 ? '1' : '0'), \
  (word & 0x1000 ? '1' : '0'), \
  (word & 0x0800 ? '1' : '0'), \
  (word & 0x0400 ? '1' : '0'), \
  (word & 0x0200 ? '1' : '0'), \
  (word & 0x0100 ? '1' : '0'), \
  (word & 0x0080 ? '1' : '0'), \
  (word & 0x0040 ? '1' : '0'), \
  (word & 0x0020 ? '1' : '0'), \
  (word & 0x0010 ? '1' : '0'), \
  (word & 0x0008 ? '1' : '0'), \
  (word & 0x0004 ? '1' : '0'), \
  (word & 0x0002 ? '1' : '0'), \
  (word & 0x0001 ? '1' : '0')
// ----------------------------------------------------------

void cxx_main_init(void)
{
	//memset(info.regs[0],0);
	int len = sizeof(info.regs) / sizeof(info.regs[0]);
	for(int i = 0 ; i < len ; i++)
	{
		info.regs[i].WORD = 0;
	}
	console_tag("CM7", "share infomation data address: 0x%X", &info);

}

void uart3_tx_work_()
{
	if (buffer.GetAmount() > 0)
	{
		if (HAL_HSEM_FastTake(HSEM_USART3) == HAL_OK)
		{
			HAL_UART_Transmit(&huart3, (uint8_t*) (buffer.GetData()), buffer.GetAmount(), 500);
			buffer.Clear();
			HAL_HSEM_Release(HSEM_USART3, 0);
		}
	}
}

void cxx_main_loop(void)
{
	console_fps("Cortex-M7");
	uart3_tx_work_();

	if( HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET )
	{
		//console_tag("CM7", "User Button Down!");
		info.regs[0].WORD |= 0x1;
	}

	EXEC_INTERVAL(1000)
	{
		//console_tag("CM7","reg[0] " WORD_TO_BINARY_PATTERN ,
		//		WORD_TO_BINARY(info.regs[0].WORD));

		info.regs[0].WORD <<= 1;
	}
	EXEC_INTERVAL_END;

	EXEC_INTERVAL(500)
	{
		static bool enable = false;
		if(enable)
		{
			HAL_GPIO_WritePin(m7_out_GPIO_Port, m7_out_Pin, GPIO_PIN_SET);
			info.regs[1].BIT._0 = 1;
		}
		else
		{
			HAL_GPIO_WritePin(m7_out_GPIO_Port, m7_out_Pin, GPIO_PIN_RESET);
			info.regs[1].BIT._0 = 0;
		}
		enable = !enable;
		//HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	}
	EXEC_INTERVAL_END

}
