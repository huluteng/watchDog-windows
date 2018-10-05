#include "watchDogService.h"
#define LOG_TAG L"watchDogService"
#include "Logger.h"



SERVICE_STATUS                      ServiceStatus;                              //����״̬
SERVICE_STATUS_HANDLE               hStatus;                                    //����״̬���

//----------------


PROCESS_INFORMATION					pi;											//�ӽ��̾��
DWORD								returnCode;									//�ӽ��̷�����
STARTUPINFO							si = { sizeof(STARTUPINFO) };

//---------------
HANDLE								hToken;										//�û�token
HANDLE								hTokenDup;									//�û�token
LPVOID								pEnv;										//������Ϣ


//���Ź���������
void WINAPI ServiceMain(DWORD argc, PWSTR* argv) {

	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
	ServiceStatus.dwWin32ExitCode = NO_ERROR;
	ServiceStatus.dwServiceSpecificExitCode = NO_ERROR;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	hStatus = RegisterServiceCtrlHandler(ServiceName, ServiceCtrlHandler);
	if (!hStatus)
	{
		DWORD dwError = GetLastError();
		log_e(_T("��������ʧ��!%d\n"), dwError);
		return;
	}

	//���÷���״̬
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);
	
	Run();

	//ֹͣ����
	ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
	SetServiceStatus(hStatus, &ServiceStatus);
}


//����ص�
void WINAPI ServiceCtrlHandler(DWORD fdwControl)
{
	switch (fdwControl) {
	case SERVICE_CONTROL_STOP:
		log_i(_T("WatchDog ����ֹͣ...\n"));
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = 0;
		SetServiceStatus(hStatus, &ServiceStatus);
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		log_i(_T("WatchDog ������ֹ...\n"));
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = 0;
		SetServiceStatus(hStatus, &ServiceStatus);
		break;
	default:
		break;
	}
}

//����ָ������
void Run() {
	log_i(_T("������óɹ�!\n"));
	DogFood * dogFood = CreateDogFood();
	log_i(_T("����״̬��:%d\n"),GetLastError());
	if (dogFood == NULL) {
		log_e(_T("��������ʧ��!\n"));
	}
	else {
		wchar_t * commandLine = ParseConfForCmd();
		BOOL flag = createProcess(commandLine);
		if (flag) {
			watching(dogFood, commandLine);
		}
		else {
			log_e(_T("��������ʧ��!\n"));
		}
		free(commandLine);
	}
	log_i(_T("ֹͣ������....\n"));
}

//���������ļ�
wchar_t * ParseConfForCmd() {
	TCHAR * path = GetFullDir();
	TCHAR * configFilePath = _tcscat(path, _T(CONFIG_FILE_PATH));
	//�ļ��ṹ��
	FILE * config_file;
	//���ļ�
	if ((config_file = _tfopen(configFilePath, _T("r"))) == NULL) {
		log_e(_T("�����ļ���ʧ��!\n"));
		exit(EXIT_FAILURE);
	}

	//��ȡ�ļ�����
	//��λ�ļ�ƫ�Ƶ�ĩβ
	fseek(config_file, 0L, SEEK_END);
	//��ȡ�ļ�����
	long total_size = ftell(config_file);
	if (total_size < 0) {
		log_e(_T("��ȡ�����ļ�ʧ��!�����ļ�����Ϊ0\n"));
		exit(EXIT_FAILURE);
	}
	//�����ڴ�
	char * json_data = malloc(sizeof(char) * total_size + 1);

	if (json_data == NULL) {
		log_e(_T("��ȡ�����ļ�ʧ��!�����ڴ�ʧ��\n"));
		exit(EXIT_FAILURE);
	}

	//�����ļ�ָ�뵽��ͷ

	fseek(config_file, 0L, SEEK_SET);

	fread(json_data, sizeof(char), total_size, config_file);

	//�ر������ļ�
	fclose(config_file);

	//����json
	cJSON *json = cJSON_Parse(json_data);

	cJSON *cmd = cJSON_GetObjectItem(json, "cmd");

	if (cmd == NULL) {
		log_e(_T("�޷���������,��������Ч!��������cmd����\n"));
		exit(EXIT_FAILURE);
	}

	//ת���ַ�
	wchar_t * commandLine = CharToWchar(cmd->valuestring);
	if (commandLine == NULL) {
		log_e(_T("�޷���������,��������Ч!ת���ֽ���Ч\n"));
		exit(EXIT_FAILURE);
	}

	//�ͷ�JSON�ַ����ڴ�
	free(json_data);
	free(configFilePath);
	return commandLine;
}

BOOL CreateProcessNoService(const wchar_t * commandLine) {
	log_i(_T("����������.....\n"));
	return  CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
}

//���񻷾��´�������
BOOL CreateProcessForService(const wchar_t * commandLine) {
	log_i(_T("����������.....\n"));
	DWORD dwSessionID = WTSGetActiveConsoleSessionId();

	//��ȡ��ǰ���ڻ״̬�û���Token
	if (!WTSQueryUserToken(dwSessionID, &hToken)) {
		int nCode = GetLastError();
		log_e(_T("��ȡ�û�tokenʧ��,������:%d\n"), nCode);
		CloseHandle(hToken);
		return FALSE;
	}

	//�����µ�Token
	if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hTokenDup)) {
		int nCode = GetLastError();
		log_e(_T("�����û�tokenʧ��,������:%d\n"), nCode);

		CloseHandle(hToken);
		return FALSE;
	}

	//����������Ϣ
	if (!CreateEnvironmentBlock(&pEnv, hTokenDup, FALSE)) {
		DWORD nCode = GetLastError();
		log_e(_T("����������Ϣʧ��,������:%d\n"), nCode);
		CloseHandle(hTokenDup);
		CloseHandle(hToken);
		return FALSE;
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = _T("winsta0\\default");

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	//��ʼ��������
	DWORD dwCreateFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT;


	if (!CreateProcessAsUser(hTokenDup, NULL, commandLine, NULL, NULL, FALSE, dwCreateFlag, pEnv, GetFullDir(), &si, &pi))
	{
		DWORD nCode = GetLastError();
		log_e(_T("��������ʧ��,������:%d\n"), nCode);
		DestroyEnvironmentBlock(pEnv);
		CloseHandle(hTokenDup);
		CloseHandle(hToken);
		return FALSE;
	}
	//����һ������
	return TRUE;
}

//�������
DogFood * CreateDogFood() {
	log_i(_T("���ɹ�����!\n"));

	PSECURITY_DESCRIPTOR pSec = (PSECURITY_DESCRIPTOR)LocalAlloc(LMEM_FIXED, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (!pSec)
	{
		return GetLastError();
	}
	if (!InitializeSecurityDescriptor(pSec, SECURITY_DESCRIPTOR_REVISION))
	{
		LocalFree(pSec);
		return GetLastError();
	}
	if (!SetSecurityDescriptorDacl(pSec, TRUE, NULL, TRUE))
	{
		LocalFree(pSec);
		return GetLastError();
	}
	SECURITY_ATTRIBUTES attr;
	attr.bInheritHandle = FALSE;
	attr.lpSecurityDescriptor = pSec;
	attr.nLength = sizeof(SECURITY_ATTRIBUTES);


	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		&attr,
		PAGE_READWRITE,
		0,
		sizeof(DogFood),
		Memory_Name
	);
	LocalFree(pSec);

	int rst = GetLastError();
	if (rst) {
		log_e(_T("�ڴ�����ʧ��!%d\n"), rst);
		return NULL;
	}
	//��ȡ����
	DogFood * dogFood = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DogFood));
	//��ʼ��
	dogFood->status = -1;
	dogFood->timestamp = 1000L;
	return dogFood;
}

//����
void watching(DogFood * dogFood, wchar_t * commandLine) {
	//���Դ���
	int re_count = 0;
	//֮ǰ״̬
	char old_status = 0;
	long old_timestamp = 0L;
	//��ʼ������
	dogFood->status = 0;
	dogFood->timestamp = 0L;
	log_i(_T("���Ź������ʼ�����,�ȴ�1���ӽ���ι������!\n"));
	//�ȴ�1����,��ʼ����ι������
	Sleep(1000 * 60);

	while (TRUE) {

		log_i(_T("������!\n"));
		//�������ʱ���Ϊ0 ���� ֹͣι������С��10,˵�������ʼ��ʧ����,����10��,ÿ�εȴ�ʱ���ӳ�һ����
		if (dogFood->timestamp == 0L && re_count < 10) {
			re_count++;
			log_e(_T("�����ʼ��ʧ��,��%d������!�ȴ�%d����...\n"), re_count, re_count + 1);
			TerminateProcess(pi.hProcess, 0);
			createProcess(commandLine);
			Sleep(1000 * 60 * (re_count + 1));
		}
		else if (dogFood->timestamp == 0L && re_count > 10) {
			//������Ȼ�ڳ�ʼ���Ĺ�����,���Ѿ�����10����,��ʱ���������Ϊ���޷�����
			log_e(_T("�����޷�����!\n"));
			TerminateProcess(pi.hProcess, 0);
			break;
		}
		else if (dogFood->timestamp == old_timestamp && dogFood->status == 1) {
			//������������,����û��ι��,��������
			log_e(_T("��ʱ��û��ι��,��������!\n"));
			re_count = 0;
			old_status = 0;
			old_timestamp = 0L;
			dogFood->status = 0;
			dogFood->timestamp = 0L;
			TerminateProcess(pi.hProcess, 0);
			int errorCode = GetLastError();
			if (errorCode) {
				log_e(_T("��������ʧ��,%d\n"),errorCode);
			}
			Sleep(1000 * 10);
			createProcess(commandLine);
			Sleep(1000 * 60);

		}
		else {
			log_i(_T("ι���ɹ�!״̬��:%d    ʱ���:%ld\n"),dogFood->status,dogFood->timestamp);
			old_status = dogFood->status;
			old_timestamp = dogFood->timestamp;
			Sleep(1000 * 60);
		}

	}
}