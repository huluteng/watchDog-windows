#include "Logger.h"

//��ȡ��ǰϵͳʱ��
TCHAR GetTime() {
	time_t t = time(0);
	//ʱ���ַ���
	static TCHAR timeStr[TIME_BUFLEN];
	_tcsftime(timeStr, TIME_BUFLEN, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return timeStr;
}

//��ӡ��־
void PrintLog(int level, TCHAR * tag, TCHAR * format, ...) {
	
	TCHAR timeStr = GetTime();
	_ftprintf(DOG_LOG,_T(""));
}