//��׼ͷ�ļ���������
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

//�Զ���ͷ�ļ���������
#include "public_func.h"
#include "ini_config.h"

//����������

//�ļ��ڲ�ʹ�õĺ�
#define UPDATE_FILE_NAME		"/ipc/update_file"
#define EXEC_FILE_NAME		"/ipc/IpcServer"

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�����

//�ڲ�����

//�ⲿ����

/*
* ��������	��	д�������ļ�
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sys_mag_write_update_file(char* buf, int len)
{
	WriteDataToFile(UPDATE_FILE_NAME,buf,len);
}

/*
* ��������	��	��������ļ��Ƿ����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sys_mag_update_check(void)
{
	int ret = access(UPDATE_FILE_NAME, F_OK);
	if (0 == ret)
	{
		//�����ļ�����
		char buf[64] = {0};
		sprintf(buf, "mv %s %s;chmod 777 %s",UPDATE_FILE_NAME,EXEC_FILE_NAME,EXEC_FILE_NAME);
		system(buf);
		sleep(1);
		system("reboot");
	}

}

/*
* ��������	��	ϵͳ����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sys_mag_reboot(void)
{
	cfg_rwlock_wrlock();
	system("reboot");
}

