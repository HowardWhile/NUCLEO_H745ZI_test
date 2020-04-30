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
extern ts_share_infomation* info;
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
	console_tag("CM4", "share infomation data address: 0x%X", info);
}

void uart3_tx_work_()
{
	if (buffer.GetAmount() > 0)
	{
		if (HAL_HSEM_FastTake(HSEM_USART3) == HAL_OK)
		{
			HAL_UART_Transmit(&huart3, (uint8_t*) (buffer.GetData()),
					buffer.GetAmount(), 500);
			buffer.Clear();
			HAL_HSEM_Release(HSEM_USART3, 0);
		}
	}
}

void cxx_main_loop(void)
{
	console_fps("Cortex-M4");
	uart3_tx_work_();

	EXEC_INTERVAL(100)
	{
		console_tag("CM4","reg[0] " WORD_TO_BINARY_PATTERN ,
				WORD_TO_BINARY(info->regs[0].WORD));

	}
	EXEC_INTERVAL_END;

	if(info->regs[1].BIT._0)
	{
		HAL_GPIO_WritePin(m4_out_GPIO_Port, m4_out_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(m4_out_GPIO_Port, m4_out_Pin, GPIO_PIN_RESET);
	}

}
