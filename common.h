#ifndef Common_H
#define Common_H


#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

//�����ڴ�����
#define Memory_Name L"ZZWTEC_SHARED_MEMORY"
//��������
#define ServiceName L"watchDogService"
//���Ź������ļ�
#define CONFIG_FILE_PATH  "/conf/config.json"
//��־·��
#define LOG_PATH  "/logs/"
//��־�ļ�����
#define LOG_FILE_PATH "log.log"
//������־·��
#define ERROR_LOG_PATH "C:\\watch_dog_error.log"
//������־�ļ�����
FILE * ERROR_LOG;
//��־�ļ�����
FILE * DOG_LOG;
#endif // !Common_H