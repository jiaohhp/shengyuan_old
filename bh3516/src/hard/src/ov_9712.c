//��׼ͷ�ļ���������

//�Զ���ͷ�ļ���������
#include "i2c_ctl.h"
#include "ov_9712.h"
#include "printf_func.h"

//����������

//�ļ��ڲ�ʹ�õĺ�

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�����
static const unsigned char 	ov9712_i2c_addr		= 0x60;
static const unsigned int  	ov9712_addr_byte		= 1;
static const unsigned int  	ov9712_data_byte		= 1;	

//�ڲ�����

/*
* ��������	��	��9712�Ĵ���
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		�Ĵ�����������
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09 V1.0		XXXX		  XXXX
*/
int ov9712_read_register(int addr)
{
	return i2c_read_data(ov9712_i2c_addr, addr, ov9712_addr_byte, ov9712_data_byte);
}

/*
* ��������	��	д9712�Ĵ���
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0		�ɹ�
										-1	ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09 V1.0		XXXX		  XXXX
*/
int ov9712_write_register(int addr, int data)
{
 	return i2c_write_data(ov9712_i2c_addr ,addr, data, ov9712_addr_byte, ov9712_data_byte);
}


//�ⲿ����


/*
* ��������	��	ģ���ʼ��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.08
* -----------------------------------------------
* 2015/08/08 V1.0		XXXX		  XXXX
*/

void ov_9712_init(void)
{
	//��ȡsensor id
	int id1,id2,id3,id4;
	ov9712_write_register(0xFF, 0x01);
	id1 = ov9712_read_register(0x1C);
	id2 = ov9712_read_register(0x1D);
	id3 = ov9712_read_register(0x0A);
	id4 = ov9712_read_register(0x0B);
	ptf_dbg("9712 id: %x %x %x %x\r\n",id1,id2,id3,id4);

	return ;
}



