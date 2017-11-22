//��׼ͷ�ļ���������
#include <pthread.h>

//�Զ���ͷ�ļ���������
#include "ini_config.h"

#include "dictionary.h"
#include "iniparser.h"
#include "printf_func.h"

//����������
char*	pastrIniFile[] 		=  {(char*)"/ipc/private.ini",(char*)"/ipc/public.ini",(char*)"/ipc/avchn_config.ini"};
char*	pastrIniFilebak[] 	=  {(char*)"/ipc/private.ini.bak",(char*)"/ipc/public.ini.bak",(char*)"/ipc/avchn_config.ini.bak"};

//�ļ��ڲ�ʹ�õĺ�

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�����
static 	dictionary	**	s_ptDicIni 		= NULL;
static	dictionary	**	s_ptDicIniBak 	= NULL;
static 	pthread_rwlock_t s_CfgMutex 	= PTHREAD_RWLOCK_INITIALIZER;		//�����ļ�������

/*
* ��������	��	�޸��ֵ��ϣ������������section��ֵ
* ��Ա����	:	��
* �������	��	type	ConfigType����
				sec		Ҫ�޸ĵ�section
				key		Ҫ�޸ĵ�key
				val		Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	0		����ɹ�
				-1		ʧ��
* �޸�����	:	2013.09.16
* -----------------------------------------------
* 2013/09/16	 V1.0		XXXX		  XXXX
*/
int cfg_set_string(ConfigType type,char* sec,char* key,char* val)
{
	int ret = iniparser_set_string(s_ptDicIni[type], sec, key, val);
	return ret;
}

/*
* ��������	��	��ȡ�ֵ��ϣ�����ַ�������section��ֵ
* ��Ա����	:	��
* �������	��	type	ConfigType����
				sec		Ҫ�޸ĵ�section
				key		Ҫ�޸ĵ�key
				val		Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	�ɹ����ز�ѯ�����ַ���
				ʧ�ܷ��� val
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
char* cfg_get_string(ConfigType type, char* sec, char* key, char* val)
{
	char* ret = iniparser_get_string(s_ptDicIni[type], sec, key, val);
	return ret;
}

/*
* ��������	��	��ȡ�ֵ��ϣ�����ַ�������section��ֵ
* ��Ա����	:	��
* �������	��	type	ConfigType����
				sec		Ҫ�޸ĵ�section
				key		Ҫ�޸ĵ�key
				val		Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	�ɹ����ز�ѯ�����ַ���
				ʧ�ܷ��� val
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
const char* cfg_get_string_bak(ConfigType type,char* sec,char* key,char* val)
{
	char* ret = iniparser_get_string(s_ptDicIniBak[type], sec, key, val);
	return ret;
}

/*
* ��������	��	�޸��ֵ��ϣ������������section��ֵ
* ��Ա����	:	��
* �������	��	type	ConfigType����
				sec		Ҫ�޸ĵ�section
				key		Ҫ�޸ĵ�key
				val		Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	0		����ɹ�
				-1		ʧ��
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int cfg_set_int(ConfigType type,char* sec,char* key,int val)
{
	int ret = iniparser_set_int(s_ptDicIni[type], sec, key, val);
	return ret;
}


/*
* ��������	��	��ȡ�ֵ��ϣ������������section��ֵ
* ��Ա����	:	��
* �������	��	type	ConfigType����
				sec		Ҫ�޸ĵ�section
				key		Ҫ�޸ĵ�key
				val		Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	�ɹ����ز�ѯ��������ֵ
				ʧ�ܷ��� val
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int cfg_get_int(ConfigType type,char* sec,char* key,int val)
{
	int ret =  iniparser_get_int(s_ptDicIni[type], sec, key, val);
	return ret;
}

/*
* ��������	��	��ȡ�ֵ��ϣ������������section��ֵ
* ��Ա����	:	��
* �������	��	type	ConfigType����
				sec		Ҫ�޸ĵ�section
				key		Ҫ�޸ĵ�key
				val		Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	�ɹ����ز�ѯ��������ֵ
				ʧ�ܷ��� val
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int cfg_get_int_bak(ConfigType type,char* sec,char* key,int val)
{
	int ret =  iniparser_get_int(s_ptDicIniBak[type], sec, key, val);
	return ret;
}

/*
* ��������	��	�����ϣ�������ļ�
* ��Ա����	:	��
* �������	��	type	ConfigType����
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	�ɹ�����	0
				ʧ�ܷ��� 	-1
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int cfg_save_ini(ConfigType type)
{
    FILE *fIni = fopen(pastrIniFile[type], "w");
    if(fIni == NULL)
    {
        ptf_err("Error: fopen< %s > is failed !!!\n",pastrIniFile[type]);
        return -1;
    }
	ptf_dbg("pastrIniFile[%d] %s",type,pastrIniFile[type]);

    iniparser_dump_ini(s_ptDicIni[type], fIni);
	ptf_dbg("pastrIniFile[%d] %s",type,pastrIniFile[type]);
    fclose(fIni);

    return 0;
}

/*
* ��������	��	�����ļ�ģ���ʼ��
* ��Ա����	:	��
* �������	��	��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void cfg_init(void)
{
	int	i 			= 0;
	int num 		= sizeof(pastrIniFile) / sizeof(pastrIniFile[0]);
    s_ptDicIni 		= (dictionary **)malloc(num * sizeof(dictionary *));
    s_ptDicIniBak 	= (dictionary **)malloc(num * sizeof(dictionary *));

	for (i = 0; i < num; i++)
	{
		s_ptDicIni[i] 		= iniparser_load(pastrIniFile[i]);
		s_ptDicIniBak[i] 	= iniparser_load(pastrIniFilebak[i]);
	}
	
    return ;
}

/*
* ��������	��	�����ļ�ģ�鷴��ʼ��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void cfg_uninit(void)
{
	int	i 			= 0;
	int num 		= sizeof(pastrIniFile) / sizeof(pastrIniFile[0]);

	for (i = 0; i < num; i++)
	{
		iniparser_freedict(s_ptDicIni[i]);
		s_ptDicIni[i] = NULL;
		iniparser_freedict(s_ptDicIniBak[i]);
		s_ptDicIniBak[i] = NULL;
	}

	free(s_ptDicIni);
	s_ptDicIni = NULL;
	free(s_ptDicIniBak);
	s_ptDicIniBak = NULL;
	
    return ;
}

/*
* ��������	��	�����ļ��ָ���������
* ��Ա����	:	��
* �������	��	��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.09.17
* -----------------------------------------------
* 2013/09/17 V1.0		XXXX		  XXXX
*/
void cfg_restore_default(void)
{	
	int cnt = 0;
	char string[256];
	int num = sizeof(pastrIniFile) / sizeof(pastrIniFile[0]);
	for (cnt = 1; cnt < num; cnt++)
	{
		memset(string, 0, sizeof(string));
		sprintf(string, "cp -a %s %s",pastrIniFilebak[cnt],pastrIniFile[cnt]);
		system(string);
	}
}

/*
* ��������	��	�Ӷ���
* ��Ա����	:	��
* �������	��	��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.09.17
* -----------------------------------------------
* 2013/09/17 V1.0		XXXX		  XXXX
*/
void cfg_rwlock_rdlock(void)
{
    pthread_rwlock_rdlock(&(s_CfgMutex));
	return ;
}

/*
* ��������	��	��д��
* ��Ա����	:	��
* �������	��	��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.09.17
* -----------------------------------------------
* 2013/09/17 V1.0		XXXX		  XXXX
*/
void cfg_rwlock_wrlock(void)
{
    pthread_rwlock_wrlock(&(s_CfgMutex));
	return ;
}

/*
* ��������	��	����
* ��Ա����	:	��
* �������	��	��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.09.17
* -----------------------------------------------
* 2013/09/17 V1.0		XXXX		  XXXX
*/
void cfg_rwlock_unlock(void)
{
    pthread_rwlock_unlock(&(s_CfgMutex));
	return ;
}
