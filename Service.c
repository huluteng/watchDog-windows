#include "Service.h"
#define LOG_TAG L"serviceInstall"
#include "Logger.h"

//���з���
void RunService() {
	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName = ServiceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	StartServiceCtrlDispatcher(ServiceTable);
}


//�ж��Ƿ��Ѿ���װ������
BOOL IsInstalled() {
	BOOL bResult = FALSE;
	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (hScm != NULL) {
		SC_HANDLE hService = OpenService(hScm, ServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL) {
			bResult = TRUE;
			CloseServiceHandle(hService);
		}
		CloseServiceHandle(hScm);
	}
	return bResult;
}

//��װ����
BOOL InstallService() {
	log_i(_T("��װ������...\n"));
	if (IsInstalled()) {
		log_i(_T("�����Ѱ�װ...\n"));
		return FALSE;
	}

	//�򿪷��������
	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (hScm == NULL) {
		log_e(_T("�򿪷��������ʧ��...\n"));
		return FALSE;
	}
	TCHAR * path = GetFullPath();
	TCHAR * arg = _T(" -s");
	TCHAR * fullPath = _tcscat(path,arg);
	SC_HANDLE hService = CreateService(hScm, 
		ServiceName, 
		ServiceName,
		SERVICE_QUERY_STATUS,
		SERVICE_WIN32_OWN_PROCESS, 
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL, 
		fullPath,
		NULL, 
		NULL, 
		NULL, 
		NULL,
		NULL);
	if (hService == NULL){
		log_e(_T("���񴴽�ʧ��...\n"));
		return FALSE;
	}
	//�ͷž��
	CloseServiceHandle(hScm);
	CloseServiceHandle(hService);
	return TRUE;
}

void Lerror(const char * format) {
	int rst = GetLastError();
	if (rst) {
		log_e(format,rst);
	}
}