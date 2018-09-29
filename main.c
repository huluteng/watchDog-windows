#pragma comment( linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )
#include <locale.h>
#include "Service.h"
#include "watchDogService.h"
#include "CUtils.h"

extern BOOL(*createProcess)(const wchar_t *);


int wmain(int argc, wchar_t *argv[]) {
	setlocale(LC_ALL, "chs");
	ERROR_LOG = _tfopen(_T(ERROR_LOG_PATH),_T("w+"));
	//û��Ȩ��,�˳�����
	if (ERROR_LOG == NULL) {
		_ftprintf(ERROR_LOG, _T("��־�ļ���ʧ��,�����˳�..."));
		exit(EXIT_FAILURE);
	}
	_ftprintf(ERROR_LOG, _T("��־�ļ���ʧ��,�����˳�..."));
	TCHAR * currentDir = GetFullDir();
	TCHAR * logPath = _tcscat(currentDir, _T(LOG_PATH));
	if (_taccess(logPath, 0) != 0) {
		_ftprintf(ERROR_LOG, _T("��־Ŀ¼������:%s,�����˳�\n"),logPath);
		exit(EXIT_FAILURE);
	}
	TCHAR * logFilePath = _tcscat(currentDir, _T(LOG_FILE_PATH));
	DOG_LOG = _tfopen(logFilePath, _T("w+"), stdout);
	if (DOG_LOG == NULL) {
		_ftprintf(ERROR_LOG, _T("��־�ļ���Ȩ�޷���:%s,�����˳�..\n"),logFilePath);
		exit(EXIT_FAILURE);
	}


	BOOL falg = InitLog();
	if (!falg) {
		_ftprintf(ERROR_LOG,_T("��־��ʼ��ʧ��!�����˳�..\n"));
		exit(EXIT_FAILURE);
	}

	//-s ��������Ѿ���װֻ��Ҫ�������񼴿�
	if ((argc > 1) && ((*argv[1] == L'-' || (*argv[1] == L'/')))) {
		if (_wcsicmp(L"s", argv[1] + 1) == 0) {
			createProcess = CreateProcessForService;
			//��������
			RunService();
		}
	}
	else {
		//���û�в������߲���-s,��ʾ����δ��װ,�Ȱ�װ����.
		BOOL result = InstallService();
		if (result) {
			//����װ�ɹ�
			log_i("����װ�ɹ�,����������..");
			createProcess = CreateProcessNoService;
			Run();
		}
		else {
			log_e("����װʧ��,�����˳�..\n");
		}
	}
	
	free(logFilePath);
	return 0;

}