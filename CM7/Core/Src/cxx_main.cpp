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

void cxx_main_init(void)
{

}

void cxx_main_loop(void)
{
	static int count = 0;
	static int err_count = 0;
	EXEC_INTERVAL(100)
	{
		console("Hello from CM7 %d err:%f", count++, sqrt(count));
	}
	EXEC_INTERVAL_END

	if(buffer.GetAmount() > 0 )
	{
		if(HAL_HSEM_FastTake(HSEM_USART3) == HAL_OK)
		{
			HAL_UART_Transmit(&huart3, (uint8_t*)buffer.GetData(), buffer.GetAmount(), 500);

			buffer.Clear();
			HAL_HSEM_Release(HSEM_USART3, 0);

		}
		else
		{
			err_count++;
		}
	}
}
