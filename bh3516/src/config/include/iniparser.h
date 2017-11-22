#ifndef _INIPARSER_H_
#define _INIPARSER_H_
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

#define ASCIILINESZ         1024
#define INI_INVALID_KEY     ((char*)-1)
#define KVCSIZE             512


/*
* ��������	��	ͨ�������ļ������ֵ��ϣ��
* ��Ա����	:	��
* �������	��	ininame	�ļ�����
* �������	��	��
* �� �� ֵ	��	ָ���ϣ���ָ��
* ����˵��	��	��
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
dictionary *iniparser_load(char * ininame);

/*
* ��������	��	����ϣ������д���ļ�
* ��Ա����	:	��
* �������	��	d	ָ���ֵ��ϣ���ָ��
					f	�ļ����
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void 	iniparser_dump_ini(dictionary * d, FILE * f);

/*
* ��������	��	�����ֵ��ϣ��
* ��Ա����	:	��
* �������	��	d	ָ���ֵ��ϣ���ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void 	iniparser_freedict(dictionary * d);
/*
* ��������	��	�޸��ֵ��ϣ�����ַ�������section��ֵ
* ��Ա����	:	��
* �������	��	d	ָ���ֵ��ϣ���ָ��
					sec	Ҫ�޸ĵ�section
					key	Ҫ�޸ĵ�key
					val	Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	0	����ɹ�
					-1	ʧ��
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int 	iniparser_set_string(dictionary *d, char* sec, char* key, char* val);

/*
* ��������	��	�޸��ֵ��ϣ������������section��ֵ
* ��Ա����	:	��
* �������	��	d	ָ���ֵ��ϣ���ָ��
					sec	Ҫ�޸ĵ�section
					key	Ҫ�޸ĵ�key
					val	Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	0	����ɹ�
					-1	ʧ��
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int 	iniparser_set_int(dictionary * d, char* sec, char* key, int  val);

/*
* ��������	��	��ȡ�ֵ��ϣ�����ַ�������section��ֵ
* ��Ա����	:	��
* �������	��	d	ָ���ֵ��ϣ���ָ��
					sec	Ҫ�޸ĵ�section
					key	Ҫ�޸ĵ�key
					val	Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	�ɹ����ز�ѯ�����ַ���
					ʧ�ܷ��� val
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
char* 	iniparser_get_string(dictionary * d, char* sec, char* key, char* val);

/*
* ��������	��	��ȡ�ֵ��ϣ������������section��ֵ
* ��Ա����	:	��
* �������	��	d	ָ���ֵ��ϣ���ָ��
					sec	Ҫ�޸ĵ�section
					key	Ҫ�޸ĵ�key
					val	Ҫ��key���õ�ֵ
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	�ɹ����ز�ѯ��������ֵ
					ʧ�ܷ��� val
* �޸�����	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int 	iniparser_get_int(dictionary * d, char* sec, char* key, int val);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
