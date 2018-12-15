#pragma comment( linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )
#include <locale.h>
#include "Service.h"
#include "watchDogService.h"
#include "CUtils.h"
#define LOG_TAG          L"main"
#include "Logger.h"

extern BOOL(*createProcess)(const wchar_t *);


int wmain(int argc, wchar_t *argv[]) {
	setlocale(LC_ALL, "chs");
	//�л�����Ŀ¼
	SetCurrentDirectory(GetFullDir());
	ERROR_LOG = _tfopen(_T(ERROR_LOG_PATH),_T("w+"));
	//û��Ȩ��,�˳�����
	if (ERROR_LOG == NULL) {
		MessageBox(NULL,_T("��־�ļ���ʧ��,�����˳�.."),_T("����"), MB_ICONSTOP);
		exit(EXIT_FAILURE);
	}
	//�رջ���
	setbuf(ERROR_LOG, NULL);
	TCHAR * currentDir = GetFullDir();
	TCHAR * logPath = _tcscat(currentDir, _T(LOG_PATH));
	if (_taccess(logPath, 0) != 0) {
		int erroCode = _mkdir(LOG_DIR_NAME);
		if (erroCode) {
			_ftprintf(ERROR_LOG, _T("��־Ŀ¼�����ڴ���Ŀ¼ʧ��:%s,�����˳�\n"), logPath);
			exit(EXIT_FAILURE);
		}
		
	}
	TCHAR * logFilePath = _tcscat(currentDir, _T(LOG_FILE_PATH));
	DOG_LOG = _tfopen(logFilePath, _T("w+"), stdout);
	if (DOG_LOG == NULL) {
		_ftprintf(ERROR_LOG, _T("��־�ļ���Ȩ�޷���:%s,�����˳�..\n"),logFilePath);
		exit(EXIT_FAILURE);
	}
	//�رջ���
	setbuf(DOG_LOG, NULL);

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
			log_i(_T("����װ�ɹ�..\n"));
		}
		else {
			log_e(_T("����װʧ��..\n"));
		}
		createProcess = CreateProcessNoService;
		Run();
	}

	free(logFilePath);
	return 0;

}