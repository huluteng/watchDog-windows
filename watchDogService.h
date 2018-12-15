#include <UserEnv.h>
#include <WtsApi32.h>
#include <sddl.h>
#include "lib/cjson/cJSON.h"
#define LOG_TAG    "dogService"
#include "CUtils.h"
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "Userenv.lib")


#ifndef Watch_Dog_Service_H
#define Watch_Dog_Service_H
//���Ź��ṹ��
typedef struct DogFood {
	//��������״̬
	char status;
	//ʱ���
	long long timestamp;
}DogFood;
//���ڴ������̵ĺ���ָ��
BOOL(*createProcess)(const wchar_t *);
#endif // !watchDogService.h




//��������
void WINAPI ServiceMain(DWORD argc, PWSTR* argv);
//�ص�����
void WINAPI ServiceCtrlHandler(DWORD);
//����ָ����GUI����
void Run();
//���������ļ�
wchar_t * ParseConfForCmd();
//�������̷������еĻ�����
BOOL CreateProcessForService(const wchar_t *);
//���������ڷǷ������еĻ�����
BOOL CreateProcessNoService(const wchar_t *);
//����ȫ�ַ���Ȩ��
//BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
//��㹷��
DogFood * CreateDogFood();
//����
void watching(DogFood *, wchar_t *);
