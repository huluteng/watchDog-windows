#include "common.h"

#define TIME_BUFLEN 64

#ifndef  LOG_TAG 
#define LOG_TAG L"main"
#endif // ! LOG_TAG 

#ifdef LOGGER_H
#define LOGGER_H


#include <time.h>
//ʱ���ַ��� ��󳤶�

enum Level{DEBUG,INFO,ERROR};
char * leveInfo[] = {"DEBUG","INFO","ERROR"};

#define log_d(...) PrintLog(DEBUG,LOG_TAG,__VA_ARGS__) 
#define log_i(...) PrintLog(INFO,LOG_TAG,__VA_ARGS__)
#define log_e(...) PrintLog(ERROR,LOG_TAG,__VA_ARGS__)
#endif // LOGGER_H


//��ӡ��־
void PrintLog(int level,TCHAR * tag,TCHAR * format,...);