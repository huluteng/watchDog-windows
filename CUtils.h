#ifndef CUTILS_H
#define CUTILS_H
#include "common.h"

//��ȡ����������·��
TCHAR *  GetFullPath();
//��ȡ��������Ŀ¼
TCHAR *  GetFullDir();
//��ʼ����־
BOOL InitLog();
//խ�ַ�ת���ַ�
wchar_t * CharToWchar(char *);
//���ַ�תխ�ַ�
char * WCharToChar(wchar_t *);

#endif // !CUTILS_h