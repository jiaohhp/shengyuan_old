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
	调用万setio,ov2640config后，9712是关闭状态，2640是开启状态
*/
int main()
{	
	
	ParaSignal();
	sys_mag_update_check();

	//加载哈希表配置文件
	cfg_init();
	
	//9712 为标准i2c
	//2640 为gpio uart模拟i2c

	//加载二进制配置文件

	//初始化只读全局变量
	global_para_init();
	
	gpio_init();
	
	ov9712_enable();
	
	//ov2640_enable();
	
	//启动网络参数模块
	netParaInit();
	
	//启动搜索器监听
	udpMBInit();

	//启动tcpserver
	tcpServerInit();
	
	//启动udp模块
	udpMBInit();
	
	//启动与采集板通信
	sample_board_init();
	
	//启动视频编码
	venc_init();
	
	//启动tcpclient
	tcpClientStart();
	
	//启动看门狗
	watch_dog_open();

	while (1)
	//int i;
	//4小时重启一次4*3600/30=480，就是i的值
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

