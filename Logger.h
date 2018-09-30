#ifndef CUTILS_H
#include "common.h"
#endif // !CUTILS_h


#ifndef LOGGER_H_
#define LOGGER_H_

#if !defined(LOG_TAG)
#define LOG_TAG          L"NO_TAG"
#endif

#include <stdarg.h>
#include <time.h>

#define __STR2WSTR(str) L##str
#define _STR2WSTR(str) __STR2WSTR(str)
#define __FUNCTIONW__ _STR2WSTR(__FUNCTION__)


/*��־����ö������*/
enum Level { L_DEBUG, L_INFO, L_ERROR };
/*��ӡdebug�������־*/
#define log_d(...) PrintLog(L_DEBUG,LOG_TAG,__FUNCTIONW__,__LINE__,__VA_ARGS__) 
/*��ӡinfo�������־*/
#define log_i(...) PrintLog(L_INFO,LOG_TAG,__FUNCTIONW__,__LINE__,__VA_ARGS__)
/*��ӡerror�������־*/
#define log_e(...) PrintLog(L_ERROR,LOG_TAG,__FUNCTIONW__,__LINE__,__VA_ARGS__)

#endif // LOGGER_H


//��ӡ��־
void PrintLog(int level,TCHAR * tag,TCHAR * func,long line,TCHAR * format,...);