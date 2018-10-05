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
	TCHAR * currentDir = GetFullDir();
	TCHAR * logPath = _tcscat(currentDir, _T(LOG_PATH));
	if (_taccess(logPath, 0) != 0) {
		MessageBox(NULL, _T("日志目录不存在,程序退出.."), _T("错误"), MB_ICONSTOP);
		exit(EXIT_FAILURE);
	}
	TCHAR * logFilePath = _tcscat(currentDir, _T(LOG_FILE_PATH));
	DOG_LOG = _tfopen(logFilePath, _T("w+"), stdout);
	if (DOG_LOG == NULL) {
		MessageBox(NULL, _T("日志文件无权限访问,程序退出.."), _T("错误"), MB_ICONSTOP);
		exit(EXIT_FAILURE);
	}
	//关闭缓冲
	setbuf(DOG_LOG, NULL);

	//-s 代表服务已经安装只需要启动服务即可
	if ((argc > 1) && ((*argv[1] == L'-' || (*argv[1] == L'/')))) {
		if (_wcsicmp(L"s", argv[1] + 1) == 0) {
			createProcess = CreateProcessForService;
			//启动服务
			RunService();
		}
	}
	else {
		//如果没有参数或者不是-s,表示服务未安装,先安装服务.
		BOOL result = InstallService();
		if (result) {
			//服务安装成功
			log_i(_T("服务安装成功,启动服务中..\n"));
			createProcess = CreateProcessNoService;
			Run();
		}
		else {
			log_e(_T("服务安装失败,程序退出..\n"));
		}
	}

	free(logFilePath);
	return 0;

}