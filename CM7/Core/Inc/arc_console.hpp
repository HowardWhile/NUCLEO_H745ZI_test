#ifndef arc_console_h__
#define arc_console_h__

#include <time.h>
#include <stdio.h>
#include <limits.h>
#define ENABLE_ARC_CONSOLE ( 1 )

/* ----------------------------- */
// 名稱: EXEC_INTERVAL
// 功能: 與EXEC_INTERVAL_END成對週期執行
// 範例:
/*	EXEC_INTERVAL(1000)
*	{
*		printf("clock() = %d \r\n", clock());
*	}
*	EXEC_INTERVAL_END;
*/
/* ----------------------------- */
#define EXEC_INTERVAL(period){\
static int check_time = INT_MIN;\
if (clock() - check_time >= period)\
{

#define EXEC_INTERVAL_END \
check_time = clock();\
}}
/* ----------------------------- */

#if ENABLE_ARC_CONSOLE == 0

#define console(...)
#define console_throttle(period, ...)
#define console_fps(tag_name)
#define console_tag(tag_name, ...)

#else
/* ----------------------------- */
// 名稱: console
// 功能: 等同於printf
/* ----------------------------- */
#define console(...)\
{\
printf("[%d ms] ", (int)clock()); \
printf(__VA_ARGS__);\
printf("\r\n");\
}
/* ----------------------------- */
// 名稱: console_tag
// 功能: 多加入tag欄位
/* ----------------------------- */
#define console_tag(tag_name, ...)\
{\
printf("[%d ms] [%s] ", (int)clock(), tag_name); \
printf(__VA_ARGS__);\
printf("\r\n");\
}
/* ----------------------------- */
// 名稱: console_throttle
// 功能: 週期輸出
/* ----------------------------- */
#define console_throttle(period, ...)\
{\
EXEC_INTERVAL(period)\
{\
console(__VA_ARGS__)\
}\
EXEC_INTERVAL_END;\
}
/* ----------------------------- */
// 名稱: console_fps
// 功能: 輸出執行處的FPS
// 格式: [TimeStamp][Tag] FPS \r\n
/* ----------------------------- */
#define console_fps(tag_name)\
{\
static int fps_count = 0;\
EXEC_INTERVAL(1000)\
{\
	console("[%s] fps: %d",\
		tag_name,\
		fps_count);\
	fps_count = 0;\
}\
EXEC_INTERVAL_END;\
fps_count++;\
}

#endif // ENABLE_ARC_CONSOLE

#endif // arc_console_h__
