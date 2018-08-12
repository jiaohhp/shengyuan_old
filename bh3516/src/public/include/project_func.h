#ifndef __PROJECT_FUNC_H__
#define __PROJECT_FUNC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdint.h>


#include "cmd_protocol.h"

typedef struct PoolDataVideo_t
{
	int							m_VFrameType;			//֡����
	unsigned long long	m_PTS;							//ʱ���	
	int 							m_VCount;					//֡����
}PoolDataVideo;

typedef enum MY_PIC_SIZE_E
{
    MY_PIC_QCIF = 0,
    MY_PIC_CIF,    
    MY_PIC_2CIF,    
    MY_PIC_HD1, 
    MY_PIC_D1,
    
    MY_PIC_QVGA,    	/* 320 * 240 */
    MY_PIC_VGA,     		/* 640 * 480 */    
    MY_PIC_XGA,     		/* 1024 * 768 */   
    MY_PIC_SXGA,    	/* 1400 * 1050 */    
    MY_PIC_UXGA,    	/* 1600 * 1200 */    
    MY_PIC_QXGA,    	/* 2048 * 1536 */

    MY_PIC_WVGA,    	/* 854 * 480 */
    MY_PIC_WSXGA,   	/* 1680 * 1050 */      
    MY_PIC_WUXGA,   	/* 1920 * 1200 */
    MY_PIC_WQXGA,   	/* 2560 * 1600 */
    
    MY_PIC_HD720,   	/* 1280 * 720 */
    MY_PIC_HD1080,  	/* 1920 * 1080 */
    MY_PIC_1280_800, 	/* 1280 * 800 */
    MY_PIC_800_600,  	/* 800 * 600 */
    MY_PIC_960H,
    MY_PIC_480H,
    
    MY_PIC_BUTT
}MY_PIC_SIZE;


typedef struct W_H_S
{
    int u32Width;
    int u32Height;
} W_H;
typedef struct POINT_S
{
    int s32X;
    int s32Y;
} POINT;

typedef struct RECT_S
{
    int s32X;
    int s32Y;
    int u32Width;
    int u32Height;
} RECT;

typedef enum PT_LOAD_TYPE_E
{    
    CMD_DATA 		= 0,       	//��������
    VIDEO_PARA 		= 1,		//��Ƶ����
    VIDEO_DATA		= 2,		//��Ƶ����
    HEART_BEAT		= 3,		//��������
    VIDEO_PICTURE	= 4,		//ͼƬ����
    SAMPLE_DATA		= 5,		//�ɼ�����
    MANUAL_SNAP_PICTURE = 6,    //xhjiao
    LAST,
}PT_LOAD_TYPE_E;

#define PT_HEAD_TAG				"BNX"		    	//�����ű�ʶͷ
#define DATA_HEAD_TAG	    	"STDH"
#define INFO_HEAD_TAG	    	"STFH"
#define FILE_UPDATE_TAG	   	"BNXD"


#define					PT_HEAD_LEN			(8)

typedef struct PT_HEAD_T
{
	unsigned char 	m_headTag[3];		//ͷ����ʶ
	unsigned char 	m_ptType;				//��������
	unsigned int			m_ptLen;					//���س���
} PT_HEAD;


#define					VIDEO_PARAM_LEN		(6)

typedef struct VIDEO_PARAM_T
{
	unsigned short 	m_width;			//ͼ����
	unsigned short	m_height;			//ͼ��߶�
	unsigned char		m_chn;				//��Ƶͨ��
	unsigned char		m_positon;		//����λ��ʶ
} VIDEO_PARAM;

#define CAP_DATA_INFO_LEN 		18

typedef struct CAPTURE_DATA_INFO_T
{
	unsigned short		m_TempNum;		//�¶����ݸ���
	unsigned short		m_HumiNum;		//ʪ�����ݸ���
	unsigned short		m_XAccNum;		//X����ٶȸ���
	unsigned short		m_YAccNum;		//Y����ٶȸ���
	unsigned short		m_ZAccNum;		//Z����ٶȸ���
	unsigned short		m_ElecNum;			//�������ݸ���
	unsigned short		m_Hydp1Num;		//Һѹ1���ݸ���
	unsigned short		m_Hydp2Num;		//Һѹ2���ݸ���
	unsigned short		m_LiqlNum;			//Һλ���ݸ���
} CAPTURE_DATA_INFO;


#define CAPTURE_PARA_LEN		9

typedef struct CAPTURE_PARAM_T
{
	unsigned char 	m_eaLimit_I;					//��������ֵ����λ
	unsigned char 	m_eaLimit_D;					//��������ֵС��λ
	unsigned char		m_accLimit_I;					//���ٶ�����ֵ����λ
	unsigned char		m_accLimit_D;					//���ٶ�����ֵС��λ

	unsigned char 	m_acRecDelayTime_I;	//����������ʱ¼��ʱ������λ
	unsigned char 	m_acRecDelayTime_D;	//����������ʱ¼��ʱ��С��λ

} CAPTURE_PARAM;

#define					UPDATE_FILE_LEN		(20)


typedef struct UPDATE_FILE_T
{
	unsigned char		m_company[4];		//�̶�Ϊ"BNXD"
	unsigned int			m_filesize;				//�����ļ���С�������ļ���Ϣͷ
	unsigned char		m_productID;	    	//��Ʒid ĿǰΪ0
	unsigned char		m_filetype;				//�����ļ�����1 �����ں�ִ�г���
	unsigned short	m_version;				//Ӳ���汾�� ĿǰΪ 0
	int64_t  				m_checksum;			//У��ͣ���������Ϣͷ��
} UPDATE_FILE;


/*
* ��������	��	���ṹ��ת���ݰ��������ֽ�����õ�buf��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		ת�����ֽ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutCaptureData2Buf(CAPTURE_DATA_INFO *psHead, unsigned char* outBuf);


/*
* ��������	��	���ṹ��ת���ݰ��������ֽ�����õ�buf��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		ת�����ֽ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutPtHead2Buf(PT_HEAD *psHead, unsigned char* outBuf);

/*
* ��������	��	��buf�������ֽ��������ת��Ϊ�����ֽ�����õ��ṹ����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		ת�����ֽ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2PtHead(PT_HEAD *psHead, unsigned char* inBuf);

/*
* ��������	��	���ṹ��ת���ݰ��������ֽ�����õ�buf��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		ת�����ֽ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutVideoPara2Buf(VIDEO_PARAM *psHead, unsigned char* outBuf);

/*
* ��������	��	��buf�������ֽ��������ת��Ϊ�����ֽ�����õ��ṹ����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		ת�����ֽ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2VideoPara(VIDEO_PARAM *psHead, unsigned char* inBuf);

/*
* ��������	��	���ṹ��ת���ݰ��������ֽ�����õ�buf��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		ת�����ֽ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutCapParam2Buf(CAPTURE_PARAM *psHead, unsigned char* outBuf);

/*
* ��������	��	��buf�������ֽ��������ת��Ϊ�����ֽ�����õ��ṹ����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2CapParam(CAPTURE_PARAM *psHead, unsigned char* inBuf);

/*
* ��������	��	���ṹ��ת���ݰ��������ֽ�����õ�buf��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		ת�����ֽ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutUpdatFile2Buf(UPDATE_FILE *psHead, unsigned char* outBuf);

/*
* ��������	��	��buf�������ֽ��������ת��Ϊ�����ֽ�����õ��ṹ����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		ת�����ֽ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2UpdatFile(UPDATE_FILE *psHead, unsigned char* inBuf);

/*
* ��������	��	���ַ������շָ�����ʽ�����ַ�������
* ��Ա����	:		��
* �������	��	strSource	Ҫ��ʽ�����ַ���
										Separator	�ָ���
										arrayNum	�����һά��С
* �������	��	stringArray	��Ÿ�ʽ������ַ���
* �� �� ֵ	��		���� 	0 	�ɹ������ַ����ɹ���ʽ���ĸ���
										С��	0 	����ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02	 V1.0		XXXX		  XXXX
*/
int  DivideStringBySeparator(char* strSource, char(*stringArray)[MAX_STRING_LEN],char Separator,int arrayNum);

/*
* ��������	��	���ݷֱ��ʻ�ȡ��Ƶ�Ŀ�͸�
* ��Ա����	:		��
* �������	��	enPicSize		�ֱ���ö������
* �������	��	pstSize		��Ƶ�Ŀ�͸�
* �� �� ֵ	��		0 	�ɹ�
										-1	ʧ��
* ����˵��	��	��
* �޸�����	:		2013.09.18
* -----------------------------------------------
* 2013/09/18	 V1.0		XXXX		  XXXX
*/
int GetPicSize(MY_PIC_SIZE enPicSize, W_H *pstSize);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

