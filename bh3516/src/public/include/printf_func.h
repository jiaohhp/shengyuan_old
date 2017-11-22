#ifndef __PRINTF_FUNC__
#define __PRINTF_FUNC__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <pthread.h>
#include <stdio.h>

#define PRINTF
#define PRINT_LOG
#define MAX_LOG_FILE_SIZE   (1024 * 256)

extern pthread_mutex_t g_print_mutex;

#define	PTF_LOCK		pthread_mutex_lock(&(g_print_mutex));
#define	PTF_UNLOCK		pthread_mutex_unlock(&(g_print_mutex));


#ifdef PRINTF
#define ptf_dbg(fmt...)	\
do{\
    PTF_LOCK\
	PrintfDataTime();\
	printf("[1;34m""[%s %s %d]""[0;39m", __FILE__, __func__,__LINE__);\
	PrintfDbg((char*)fmt);\
	PTF_UNLOCK\
} while(0)
#else
#define ptf_dbg(fmt...)	\
do{\
} while(0)
#endif


#ifdef PRINTF
#define ptf_err(fmt...)	\
do{\
    PTF_LOCK\
	PrintfDataTime();\
	printf("[1;31m""[%s %s %d]""[0;39m", __FILE__, __func__,__LINE__);\
	PrintfErr((char*)fmt);\
	PTF_UNLOCK\
} while(0)
#else
#define ptf_err(fmt...)	\
do{\
} while(0)
#endif


#ifdef PRINTF
#define ptf_suc(fmt...)	\
do{\
    PTF_LOCK\
	PrintfDataTime();\
	printf("[1;32m""[%s %s %d]""[0;39m", __FILE__, __func__,__LINE__);\
	PrintfSuc((char*)fmt);\
	PTF_UNLOCK\
} while(0)
#else
#define ptf_suc(fmt...)	\
do{\
} while(0)
#endif

#ifdef PRINT_LOG
#define ptf_log(fmt...)	\
do{\
    PTF_LOCK\
	PrintfLog((char*)fmt);\
	PTF_UNLOCK\
} while(0)
#else
#define ptf_log(fmt...)	\
do{\
} while(0)
#endif


#ifdef PRINTF
#define ptf_fix_len_string(buf,len)	\
do{\
    PTF_LOCK\
	PrintfDataTime();\
	printf("[1;34m""[%s %s %d]""[0;39m", __FILE__, __func__,__LINE__);\
	PrintfFixString(buf,len);\
	PTF_UNLOCK\
} while(0)
#else
#define ptf_fix_len_string(buf,len) \
do{\
} while(0)
#endif


void 	PrintfDataTime(void);
void 	PrintfFixString(char* buf, int len);
int 	PrintfSuc(char* pszfmt, ...);
int 	PrintfDbg(char* pszfmt, ...);
int 	PrintfErr(char* pszfmt, ...);
int 	PrintfLog(char* pszfmt, ...);
int 	SetLogFilePath(char* pcFileName,int len);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

