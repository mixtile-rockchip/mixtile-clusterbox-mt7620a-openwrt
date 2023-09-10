#ifndef __LOG_H__
#define __LOG_H__
 
typedef enum 
{ 
	LOG_ERROR = 1, 
	LOG_WARN = 2, 
	LOG_INFO = 3, 
	LOG_DEBUG = 4 
} LOG_LEVEL;
 
// #define LOG_LEVEL_DBG LOG_DEBUG 
  
#ifdef LOG_LEVEL_DBG
#define _Log_Gen(file, func, line, level, levelStr, fmt, ...)  \
do {  \
    if (level <= LOG_LEVEL_DBG) {printf("[%s %s]: %s|%s|%s()(%d) "fmt"\n", __DATE__, __TIME__, levelStr, file, func, line, ##__VA_ARGS__);}  \
} while(0)
#else 
#define _Log_Gen(file, func, line, level, levelStr, fmt, ...)  
#endif 
  
/* Simple APIs for log generation in different levels */ 
#define Log_d(fmt, ...) _Log_Gen(__FILE__, __FUNCTION__, __LINE__, LOG_DEBUG, "DBG", fmt, ##__VA_ARGS__) 
#define Log_i(fmt, ...) _Log_Gen(__FILE__, __FUNCTION__, __LINE__, LOG_INFO, "INF",fmt, ##__VA_ARGS__) 
#define Log_w(fmt, ...) _Log_Gen(__FILE__, __FUNCTION__, __LINE__, LOG_WARN, "WAR",fmt, ##__VA_ARGS__) 
#define Log_e(fmt, ...) _Log_Gen(__FILE__, __FUNCTION__, __LINE__, LOG_ERROR, "ERR", fmt, ##__VA_ARGS__)

#endif
