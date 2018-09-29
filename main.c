#pragma comment( linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )
#include <stdio.h>
#include "Service.h"
#include "watchDogService.h"
#include "CUtils.h"

int wmain(int argc, wchar_t *argv[]) {
	extern BOOL(*createProcess)(const wchar_t *);

	FILE * error_log_file = _tfreopen(_T("C:\\watch_dog_error.log"), _T("w+"), stderr);
	BOOL falg = InitLog();
	if (!falg) {
		fprintf(stderr,"��־��ʼ��ʧ��!�����˳�..\n");
		fclose(error_log_file);
		return -2;
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
			fclose(error_log_file);
		}
	}
	fclose(error_log_file);
	return 0;

}