//标准头文件包含区域
#include <string.h>

//自定义头文件包含区域
#include "project_func.h"
#include "public_func.h"
#include "printf_func.h"

//常量定义区

//文件内部使用的宏

//文件内部使用的数据类型

//全局变量

//静态局部变量

//内部函数

//外部函数

/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutPtHead2Buf(PT_HEAD *psHead, unsigned char* outBuf)
{
	outBuf[0] = psHead->m_headTag[0];
	outBuf[1] = psHead->m_headTag[1];
	outBuf[2] = psHead->m_headTag[2];
	outBuf[3] = psHead->m_ptType;
	sendChange((char*)&(psHead->m_ptLen),(char*)outBuf + 4,sizeof(psHead->m_ptLen));
	//遵循网络字节序，先传输高位数据
	return PT_HEAD_LEN;
}

/*
* 功能描述	：	将buf中网络字节序的数据转化为主机字节序放置到结构体中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2PtHead(PT_HEAD *psHead, unsigned char* inBuf)
{
	psHead->m_headTag[0] 	= inBuf[0];
	psHead->m_headTag[1] 	= inBuf[1];
	psHead->m_headTag[2] 	= inBuf[2];
	psHead->m_ptType 			= inBuf[3];
	recvChange((char*)inBuf + 4,(char*)&(psHead->m_ptLen),sizeof(psHead->m_ptLen));
	return 8;
}

/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutVideoPara2Buf(VIDEO_PARAM *psHead, unsigned char* outBuf)
{
	sendChange((char*)&(psHead->m_width), (char*)outBuf,sizeof(psHead->m_width));
	sendChange((char*)&(psHead->m_height),(char*)outBuf + 2,sizeof(psHead->m_height));
	outBuf[4] = psHead->m_chn;
	outBuf[5] = psHead->m_positon;
	return 6;
}

/*
* 功能描述	：	将buf中网络字节序的数据转化为主机字节序放置到结构体中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2VideoPara(VIDEO_PARAM *psHead, unsigned char* inBuf)
{
	recvChange((char*)inBuf, (char*)&(psHead->m_width),sizeof(psHead->m_width));
	recvChange((char*)inBuf + 2, (char*)&(psHead->m_height),sizeof(psHead->m_height));
	psHead->m_chn 			= inBuf[4];
	psHead->m_positon 	= inBuf[5];
	return 6;
}

/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutCaptureData2Buf(CAPTURE_DATA_INFO *psHead, unsigned char* outBuf)
{
	outBuf[0] 		= (unsigned char)(psHead->m_TempNum >> 8);
	outBuf[1] 		= (unsigned char)(psHead->m_TempNum);
	outBuf[2] 		= (unsigned char)(psHead->m_HumiNum >> 8);
	outBuf[3] 		= (unsigned char)(psHead->m_HumiNum);
	outBuf[4] 		= (unsigned char)(psHead->m_XAccNum >> 8);
	outBuf[5] 		= (unsigned char)(psHead->m_XAccNum);
	outBuf[6] 		= (unsigned char)(psHead->m_YAccNum >> 8);
	outBuf[7] 		= (unsigned char)(psHead->m_YAccNum);
	outBuf[8] 		= (unsigned char)(psHead->m_ZAccNum >> 8);
	outBuf[9] 		= (unsigned char)(psHead->m_ZAccNum);
	outBuf[10] 	= (unsigned char)(psHead->m_ElecNum >> 8);
	outBuf[11] 	= (unsigned char)(psHead->m_ElecNum);
	outBuf[12] 	= (unsigned char)(psHead->m_Hydp1Num >> 8);
	outBuf[13] 	= (unsigned char)(psHead->m_Hydp1Num);
	outBuf[14] 	= (unsigned char)(psHead->m_Hydp2Num >> 8);
	outBuf[15] 	= (unsigned char)(psHead->m_Hydp2Num);
	outBuf[16] 	= (unsigned char)(psHead->m_LiqlNum >> 8);
	outBuf[17] 	= (unsigned char)(psHead->m_LiqlNum);

	return 18;
}

/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutCapParam2Buf(CAPTURE_PARAM *psHead, unsigned char* outBuf)
{
	outBuf[0] 	= psHead->m_eaLimit_I;
	outBuf[1] 	= psHead->m_eaLimit_D;
	outBuf[2] 	= psHead->m_accLimit_I;
	outBuf[3] 	= psHead->m_accLimit_D;
	outBuf[4] 	= psHead->m_acRecDelayTime_I;
	outBuf[5] 	= psHead->m_acRecDelayTime_D;

	return 6;

}

/*
* 功能描述	：	将buf中网络字节序的数据转化为主机字节序放置到结构体中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2CapParam(CAPTURE_PARAM *psHead, unsigned char* inBuf)
{
	psHead->m_eaLimit_I 			= inBuf[0];
	psHead->m_eaLimit_D 		= inBuf[1];
	psHead->m_accLimit_I		= inBuf[2];
	psHead->m_accLimit_D		= inBuf[3];
	psHead->m_acRecDelayTime_I	= inBuf[4];
	psHead->m_acRecDelayTime_D	= inBuf[5];
	
	return 6;
}

/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutUpdatFile2Buf(UPDATE_FILE *psHead, unsigned char* outBuf)
{
	outBuf[0] = psHead->m_company[0];
	outBuf[1] = psHead->m_company[1];
	outBuf[2] = psHead->m_company[2];
	outBuf[3] = psHead->m_company[3];
	sendChange((char*)&(psHead->m_filesize), (char*)outBuf + 4, sizeof(psHead->m_filesize));
	outBuf[8] = psHead->m_productID;
	outBuf[9] = psHead->m_filetype;
	sendChange((char*)&(psHead->m_version),(char*)outBuf + 10,sizeof(psHead->m_version));
	sendChange((char*)&(psHead->m_checksum),(char*)outBuf + 12,sizeof(psHead->m_checksum));

	return 20;
}

/*
* 功能描述	：	将buf中网络字节序的数据转化为主机字节序放置到结构体中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2UpdatFile(UPDATE_FILE *psHead, unsigned char* inBuf)
{
	psHead->m_company[0] = inBuf[0];
	psHead->m_company[1] = inBuf[1];
	psHead->m_company[2] = inBuf[2];
	psHead->m_company[3] = inBuf[3];

	recvChange((char*)inBuf + 4,(char*)&(psHead->m_filesize),sizeof(psHead->m_filesize));
	
	psHead->m_productID = inBuf[8];
	psHead->m_filetype  = inBuf[9];
	
	recvChange((char*)inBuf + 10,(char*)&(psHead->m_version),sizeof(psHead->m_version));
	recvChange((char*)inBuf + 12,(char*)&(psHead->m_checksum),sizeof(psHead->m_checksum));

	return 20;
}

/*
* 功能描述	：	将字符串按照分隔符格式化到字符串数组
* 成员更新	:		无
* 输入参数	：	strSource	要格式化的字符串
										Separator	分隔符
										arrayNum	数组的一维大小
* 输出参数	：	stringArray	存放格式化后的字符串
* 返 回 值	：		大于 	0 	成功返回字符串成功格式化的个数
										小于	0 	返回失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02	 V1.0		XXXX		  XXXX
*/
int  DivideStringBySeparator(char* strSource, char(*stringArray)[MAX_STRING_LEN],char Separator,int arrayNum)
{
    char* start 	= (char*)strSource;
    char* find 		= NULL;
    int i 		= 0;
    int len 	= 0;


    while (NULL != (find = strchr(start,Separator)))
    {
        len = find - start;
        if (len >= MAX_STRING_LEN)
        {
            ptf_err("len %d strSource %s ",len,strSource);
            return -1;
        }

        if (i >= arrayNum)
        {
            ptf_err("Err: i will big arrayNum.  i=%d, arrayNum=%d",i,arrayNum); 
            return -2;
        }

        strncpy((char*)&(stringArray[i++]),start,len);
        start = find + 1;
    }

    if (i != arrayNum)
    {
    	 ptf_err("i %d arrayNum %d strSource %s",i,arrayNum,strSource);
        return -3;
    }
    return i;
}

/*
* 功能描述	：	根据分辨率获取视频的宽和高
* 成员更新	:		无
* 输入参数	：	enPicSize		分辨率枚举类型
* 输出参数	：	pstSize		视频的宽和高
* 返 回 值	：		0 	成功
										-1	失败
* 其它说明	：	无
* 修改日期	:		2013.09.18
* -----------------------------------------------
* 2013/09/18	 V1.0		XXXX		  XXXX
*/
int GetPicSize(MY_PIC_SIZE enPicSize, W_H *pstSize)
{
    switch (enPicSize)
    {
    case MY_PIC_QCIF:
        pstSize->u32Width 	= 176;
        pstSize->u32Height 	= 144;
        break;
    case MY_PIC_CIF:
        pstSize->u32Width 	= 352;
        pstSize->u32Height 	= 288;
        break;
    case MY_PIC_D1:
        pstSize->u32Width 	= 704;
        pstSize->u32Height 	= 576;
        break;
    case MY_PIC_960H:
        pstSize->u32Width 	= 960;
        pstSize->u32Height 	= 540;
        break;
    case MY_PIC_480H:
        pstSize->u32Width 	= 480;
        pstSize->u32Height 	= 270;
        break;
    case MY_PIC_QVGA:	  	/* 320 * 240 */
        pstSize->u32Width 	= 320;
        pstSize->u32Height 	= 240;
        break;
    case MY_PIC_VGA:	  	/* 640 * 480 */
        pstSize->u32Width 	= 640;
        pstSize->u32Height 	= 480;
        break;
    case MY_PIC_XGA:	  	/* 1024 * 768 */
        pstSize->u32Width 	= 1024;
        pstSize->u32Height 	= 768;
        break;
    case MY_PIC_SXGA:	  	/* 1400 * 1050 */
        pstSize->u32Width 	= 1400;
        pstSize->u32Height 	= 1050;
        break;
    case MY_PIC_UXGA:	  	/* 1600 * 1200 */
        pstSize->u32Width 	= 1600;
        pstSize->u32Height 	= 1200;
        break;
    case MY_PIC_QXGA:	  	/* 2048 * 1536 */
        pstSize->u32Width 	= 2048;
        pstSize->u32Height 	= 1536;
        break;
    case MY_PIC_WVGA:	  	/* 854 * 480 */
        pstSize->u32Width 	= 854;
        pstSize->u32Height 	= 480;
        break;
    case MY_PIC_WSXGA:   	/* 1680 * 1050 */
        pstSize->u32Width 	= 1680;
        pstSize->u32Height 	= 1050;
        break;
    case MY_PIC_WUXGA:   	/* 1920 * 1200 */
        pstSize->u32Width 	= 1920;
        pstSize->u32Height 	= 1200;
        break;
    case MY_PIC_WQXGA:   	/* 2560 * 1600 */
        pstSize->u32Width 	= 2560;
        pstSize->u32Height 	= 1600;
        break;
    case MY_PIC_HD720:   	/* 1280 * 720 */
        pstSize->u32Width 	= 1280;
        pstSize->u32Height 	= 720;
        break;
    case MY_PIC_HD1080:  	/* 1920 * 1080 */
        pstSize->u32Width 	= 1920;
        pstSize->u32Height 	= 1080;
        break;
    default:
        return -1;
    }
    return 0;
}
