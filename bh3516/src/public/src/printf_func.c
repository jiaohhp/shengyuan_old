
//±ê×¼Í·ÎÄ¼þ°üº¬ÇøÓò
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

//×Ô¶¨ÒåÍ·ÎÄ¼þ°üº¬ÇøÓò
#include "printf_func.h"

//³£Á¿¶¨ÒåÇøÓò
pthread_mutex_t g_print_mutex = PTHREAD_MUTEX_INITIALIZER;

static 		char *pfile	= NULL;

//ÎÄ¼þÄÚ²¿Ê¹ÓÃµÄºê

void PrintfDataTime(void)
{
    time_t now;
    struct tm *ptm_now;
    now = time(&now);
    ptm_now = gmtime(&now);
	
    printf("[1;33m""[%04d/%02d/%02d %02d:%02d:%02d]""[0;39m",
           ptm_now->tm_year + 1900,
           ptm_now->tm_mon + 1,
           ptm_now->tm_mday,
           (ptm_now->tm_hour) % 24,
           ptm_now->tm_min,
           ptm_now->tm_sec);
	printf("[pth id %4ld]",syscall(224));

    return ;

}

void PrintfFixString(char* buf, int len)
{
    char TmpBuf[len + 1];
    TmpBuf[len] = '\0';
    memcpy(TmpBuf, buf, len);
    //printf(" %s\n", TmpBuf);
}

int PrintfSuc(char* pszfmt, ...)
{
	if(pszfmt == NULL)
    {
        return -1;
    }

    if((strlen(pszfmt) + 3) > 512)
    {
        return -2;
    }
		
    va_list struAp;
    int ret;
    char format[512];


    memset(format, 0, sizeof(format));
    strcpy(format, pszfmt);
    if(format[strlen(pszfmt) - 1] == '\n')
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt) - 2] == '\r' ? '\0' : '\r';
    }
    else if(format[strlen(pszfmt) - 1] == '\r')
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt) - 2] == '\n' ? '\0' : '\n';
    }
    else
    {
        format[strlen(pszfmt)] = '\r';
        format[strlen(pszfmt) + 1] = '\n';
    }

    va_start(struAp, pszfmt);
    ret = vprintf(format, struAp);
    va_end(struAp);
	
    return ret;
}

int PrintfDbg(char* pszfmt, ...)
{		
	if(pszfmt == NULL)
    {
        return -1;
    }

    if((strlen(pszfmt) + 3) > 512)
    {
        return -2;
    }
		
    va_list struAp;
    int ret;
    char format[512];


    memset(format, 0, sizeof(format));
    strcpy(format, pszfmt);
    if(format[strlen(pszfmt) - 1] == '\n')
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt) - 2] == '\r' ? '\0' : '\r';
    }
    else if(format[strlen(pszfmt) - 1] == '\r')
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt) - 2] == '\n' ? '\0' : '\n';
    }
    else
    {
        format[strlen(pszfmt)] = '\r';
        format[strlen(pszfmt) + 1] = '\n';
    }

    va_start(struAp, pszfmt);
    ret = vprintf(format, struAp);
    va_end(struAp);
	
    return ret;
}

int PrintfErr(char* pszfmt, ...)
{	
    if(pszfmt == NULL)
    {
        return -1;
    }

    if((strlen(pszfmt) + 3) > 512)
    {
        return -2;
    }
			
    va_list struAp;
    time_t now;
    struct tm *ptm_now;
    int ret;
    char format[512];

    memset(format, 0, sizeof(format));
    strcpy(format, pszfmt);
    if(format[strlen(pszfmt) - 1] == '\n')
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt) - 2] == '\r' ? '\0' : '\r';
    }
    else if(format[strlen(pszfmt) - 1] == '\r')
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt) - 2] == '\n' ? '\0' : '\n';
    }
    else
    {
        format[strlen(pszfmt)] = '\r';
        format[strlen(pszfmt) + 1] = '\n';
    }
    now = time(&now);
    ptm_now = gmtime(&now);

    va_start(struAp, pszfmt);
    ret = vprintf(format, struAp);
    va_end(struAp);
	
    return ret;
}

int SetLogFilePath(char* pcFileName,int len)
{

	if (pfile == NULL)
	{
		pfile = (char*)malloc(len + 1);
		if (NULL == pfile)
		{
			return -1;
		}

		memset(pfile, 0, len + 1);
		memcpy(pfile, pcFileName, len);
	}
	else
	{
		return -2;
	}

	return 0;
}

int PrintfLog(char* pszfmt, ...)
{
	if (pfile == NULL)
	{
		return -3;
	}
	
	pthread_mutex_lock(&g_print_mutex);
	
    va_list struAp;
    time_t now;
    struct tm *ptm_now;
    int ret;
    char format[512];

    if(pszfmt == NULL)
    {
    	pthread_mutex_unlock(&g_print_mutex);
        return -1;
    }

    if((strlen(pszfmt) + 3) > 512)
    {
    	pthread_mutex_unlock(&g_print_mutex);
        return -2;
    }
    memset(format, 0, sizeof(format));
    strcpy(format, pszfmt);
    if(format[strlen(pszfmt) - 1] == '\n')
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt) - 2] == '\r' ? '\0' : '\r';
    }
    else if(format[strlen(pszfmt) - 1] == '\r')
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt) - 2] == '\n' ? '\0' : '\n';
    }
    else
    {
        format[strlen(pszfmt)] = '\r';
        format[strlen(pszfmt) + 1] = '\n';
    }
    now = time(&now);
    ptm_now = gmtime(&now);

    FILE *file;

    file = fopen(pfile, "a+");
    fprintf(file, "[%04d/%02d/%02d %02d:%02d:%02d]",
            ptm_now->tm_year + 1900,
            ptm_now->tm_mon + 1,
            ptm_now->tm_mday,
            (ptm_now->tm_hour) % 24,
            ptm_now->tm_min,
            ptm_now->tm_sec);

    va_start(struAp, pszfmt);
    ret = vfprintf(file, format, struAp);
    va_end(struAp);
    fclose(file);
    struct stat buf;
    if (stat(pfile, &buf) == -1)
    {
        perror("stat");
    }
    else
    {
        if (buf.st_size > MAX_LOG_FILE_SIZE)
        {
        	char buf[128] = {0};
        	sprintf(buf,"rm -rf %s_bck",pfile);
            system(buf);
			memset(buf, 0, sizeof(buf));
        	sprintf(buf,"mv %s %s_bck",pfile,pfile);
			system(buf);
        }
    }
    
    pthread_mutex_unlock(&g_print_mutex);
    
    return ret;
}

