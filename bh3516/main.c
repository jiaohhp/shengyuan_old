#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "ini_config.h"
#include "global_para.h"
#include "net_para.h"
#include "tcp_server.h"
#include "sample_board.h"
#include "watch_dog.h"
#include "udp_broadcast.h"
#include "gpio_cfg.h"
#include "ov_9712.h"
#include "ov_2640.h"
#include "chn_venc.h"
#include "sys_mag.h"
#include "tcp_client.h"
#include "public_func.h"
#include "dev_property.h"
/*
	������setio,ov2640config��9712�ǹر�״̬��2640�ǿ���״̬
*/
int main()
{	
	
	ParaSignal();
	sys_mag_update_check();

	//���ع�ϣ�������ļ�
	cfg_init();
	
	//9712 Ϊ��׼i2c
	//2640 Ϊgpio uartģ��i2c

	//���ض����������ļ�

	//��ʼ��ֻ��ȫ�ֱ���
	global_para_init();
	
	gpio_init();
	
	ov9712_enable();
	
	//ov2640_enable();
	
	//�����������ģ��
	netParaInit();
	
	//��������������
	udpMBInit();

	//����tcpserver
	tcpServerInit();
	
	//����udpģ��
	udpMBInit();
	
	//������ɼ���ͨ��
	sample_board_init();
	
	//������Ƶ����
	venc_init();
	
	//����tcpclient
	tcpClientStart();
	
	//�������Ź�
	watch_dog_open();

	while (1)
	//int i;
	//4Сʱ����һ��4*3600/30=480������i��ֵ
	//for(i=0;i<120||sleep(300);i++)
	{
		//char qutichar = getchar();
		//if (qutichar == 'q')
		//{
		//	//ptf_dbg("------------");
		//	break;
		//}else
		//{
			watch_dog_keeplive();
			sleep(30);
			ptf_dbg("feed dogs");
		//}
	}

	return 0;
}

