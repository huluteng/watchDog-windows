#include "Logger.h"

//��־����󳤶�
#define LOG_MAX_BUF_SIZE 1024
/*ʱ���ַ������64*/
#define TIME_BUFLEN 64
#define TIME_FORMAT L"%Y-%m-%d %H:%M:%S"
/*��־ǰ�沿�ֵĸ�ʽ [ʱ��]  ��־����  ��ǩ*/
#define LOG_FORMAT L"[%20s %s] %s/%s %s:%ld:   "

static TCHAR * leveInfo[] = { L"D",L"I",L"E" };
static TCHAR logBuf[LOG_MAX_BUF_SIZE];


//��ȡ��ǰϵͳʱ��
TCHAR* GetTime() {
	static TCHAR timeStr[TIME_BUFLEN];
	time_t t = time(NULL);
	struct tm * local = localtime(&t);
	_tcsftime(timeStr, TIME_BUFLEN, TIME_FORMAT, local);
	return timeStr;
}
//��ȡ����ID
TCHAR *GetPID() {
	static TCHAR pidBuf[10];
	long pid = GetCurrentProcessId();
	_stprintf(pidBuf, L"PID:%04ld", GetCurrentProcessId());

	return pidBuf;
}



//��ӡ��־
void PrintLog(int level, TCHAR * tag, TCHAR * func, long line, TCHAR * format, ...) {
	//ʱ��
	TCHAR* timeStr = GetTime();
	//��־�ȼ�
	TCHAR* levelStr = leveInfo[level];
	TCHAR* pid = GetPID();
	int index = _stprintf(logBuf, LOG_FORMAT,timeStr, pid, levelStr,tag, func,line);
	va_list args;
	va_start(args,format);
	_vsntprintf(logBuf + index, LOG_MAX_BUF_SIZE - index, format,args);
	va_end(args);
	//��ӡ��־
	_ftprintf(DOG_LOG,logBuf);

}