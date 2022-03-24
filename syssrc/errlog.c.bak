#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <ctype.h>
#include <linux/errno.h>
#include <errno.h>

#include "../include/errlog.h"

/*static  int   PbLogLvl = 0;*/
static int LogLvl = 0;

#define FILENAME_LEN			80						/*文件名长度*/
#define PER_MAX_LOG_FILE		20						/*每笔最大日志文件数*/
#define LOG_BUFF_SIZE			2*1024*1024				/*日志缓存空间*/
#define MAXNAMELEN				200						/*日志文件路径*/

static char caPerLogFile[PER_MAX_LOG_FILE][FILENAME_LEN];	/**/
static char caLogBuff[LOG_BUFF_SIZE];


struct LogNode{
	int		iLogLvl;									/* 日志级别 */
	int		iLogLen;									/* 日志长度 */
	int		iFileNo;									/* 日志文件序号 */
	char	*pLogInfo;									/* 日志信息 */
	struct LogNode *next;								/* 下一个日志节点 */
};

static struct LogHead{
	int 	iLogStatus;
	int		iFlag;
	int		iLogNormal;
	int		iLogError;
	struct	LogNode	*pNode;
}stLogHead;

static char 	*pMemHead = NULL;
static long 	lUsedSize = 0;
static int		iUseNewLog = 0;
//static int 		iUseNewLog = 0;
static struct LogNode *pEndNode = NULL;
static int BuffMode = 0;
static int LogFileSize = 2048;

int AddLogNode(int iLvl, char *pcFileName, char *pcMsg, int iMsgLen);
struct LogNode * WriteLogNode(struct LogNode *pNode);
char * WriteLogInfo(char *pcInfo, int iSize, int *ipFlag);
int pbSetlog(int loglvl);
int ChkFileSize(FILE* fp, char *filename);
struct LogNode * WriteLogNode(struct LogNode *pNode);
int logbasemsg(char *filename, char *fmt, ...);
void CommitLog(int iInfo);
void ClearLogBuff(void);
//int GetLoglvl();

int pbSetlog(int loglvl)
{
	LogLvl = loglvl;
	return (loglvl);
}

int pbgetlog()
{
	return (LogLvl);
}
char* GetEnv(char* envname)
{
	static char tmp[10];
	static char* ret;
	tmp[0]='\0';
	if(envname != NULL)
	{
		ret = getenv(envname);
		if(ret != NULL)
			return ret;
		else    
			return tmp;
	}
	else    
		return tmp;
}

int WriteLog(int loglev, char * filename1, const char * func, int line, char * infolev, char * fmt, ... )
//int WriteLog_new(int loglev, char * filename1, const char * func, int line, char * fmt, ... )
{
    //PbLogLvl = // = getlogLvl();
    if ( loglev >  pbgetlog() ) 
        return -1;
    va_list args;
    FILE    *logfp;
    char    *fopntype = "a";
    char    filep[ 200 ] ;
	char    strBlank[10] ;
	char    strLine[256] ;
	char 	filename[80];
	struct  tm   *tm;
    struct  timeval evt_timestamp;


	memset( filename, 0, sizeof( filename));
	memcpy( filename, filename1 , 12);

    sprintf( filep , "%s/%s.log" , getenv("LOGDIR") , logfile ) ;

    logfp = fopen ( filep, fopntype );
    if ( ( logfp == NULL ) && ( logfp = stderr ) == NULL )
    {
        fprintf(stderr,"Log File %s open Error!\n",filep);
        return -1;
    }
	setbuf( logfp , (char *)NULL ) ;
	/*
	 *  鍥犺澶寸殑闀跨煭涓嶄竴锛屾晠鎸0浣嶈繘琛屽榻愯ˉ鍋	*/
    /*sprintf(strLine, "[%ld|%s|%-13s:%4d]", getpid(),GetSystemTime(),filename,line );*/
    memset( &evt_timestamp, 0x00, sizeof( evt_timestamp ) );
    gettimeofday( &evt_timestamp, NULL );
    tm = (struct tm *)localtime( &evt_timestamp.tv_sec );

	if(func == NULL)
		sprintf(strLine, "%d|%02d%02d%02d.%03ld|%s:%d-%s",getpid(), tm->tm_hour, tm->tm_min, tm->tm_sec,evt_timestamp.tv_usec/1000, filename,  line, infolev);
	else
		sprintf(strLine, "%d|%02d%02d%02d.%03ld|%s(%s):%d-%s",getpid(), tm->tm_hour, tm->tm_min, tm->tm_sec,evt_timestamp.tv_usec/1000, filename, func, line, infolev);
    //sprintf(strLine, "%d|%02d%02d%02d.%03ld|%s(%s):%d",getpid(), tm->tm_hour, tm->tm_min, tm->tm_sec,evt_timestamp.tv_usec/1000, filename, func, line);

	switch( ( strlen( strLine ) + 4 ) /4 )
	{
		case 1 :
			strcpy( strBlank , "\t\t\t\t\t\0" ) ;
			break ;
		case 2 :
			strcpy( strBlank , "\t\t\t\t\0" ) ;
			break ;
		case 3 :
			strcpy( strBlank , "\t\t\t\0" ) ;
			break ;
		case 4 :
			strcpy( strBlank , "\t\t\0" ) ;
			break ;
		case 5 :
			strcpy( strBlank , "\t\0" ) ;
			break ;
		default :
			strcpy( strBlank , "\0" ) ;
			break ;
	}
	/* 鏍煎紡涓插彲浠ュ寘鍚墠瀵兼崲琛岀 */
	while( *(fmt++) == '\n' ) fprintf(logfp, "\n");
	fmt-- ;

    fprintf(logfp, "%s%s: ", strLine , strBlank ) ;

    va_start( args , fmt );
    vfprintf(logfp, fmt, args);
    va_end( args );

    fprintf(logfp, "\n");

    if ( logfp != stderr )
        fclose ( logfp );

    return 0;
}


int GetLogFile( char *filep)
{
	char *envpar = NULL;
	static char *path = NULL;
	
	if(path == NULL)
	{
		if( ( path = (char*)malloc(MAXNAMELEN) ) != NULL)
		{
			envpar = (char*)GetEnv("HOME");
			if( strlen(envpar) >0 )
				sprintf(path, "%s/log", envpar);
		}
	}
	
	if(path != NULL)
		sprintf(filep, "%s/%s", path, logfile);
	else
		return -1;
		
	return 0;
}

int WriteHexLogNow(int loglev, char *filename, int line, unsigned char* str, int len)
{
    filename = filename;
    line = line;
	int i = 0;
	long pkg_len, j, h;
	char filep[MAXNAMELEN];
	char filepfull[MAXNAMELEN];
	char tmpmsg[512];
	char chl[5];
	
	memset(filep, 0,  sizeof(filep));
	memset(filepfull, 0,  sizeof(filepfull));
	
	if(loglev > pbgetlog())
	{
		WriteLog(ERR_LVL, ERROR, "pbgetlog()=[%d]", pbgetlog());
		return 0;
	}
	
	GetLogFile(filep);
	
	if(strstr(filep, ".log") != NULL)
		sprintf(filepfull, "%s", filep);
	else
		sprintf(filepfull, "%s.log", filep);

	WriteLog(ERR_NOW,INFO, "==%8d==", getpid());
	logbasemsg(filepfull, "%d|DataLen=%d\n", getpid(), len);
	
	pkg_len = len;
	
	for(i=0; i<(pkg_len / 16 + 1); i++)
	{
		memset(tmpmsg, 0, sizeof(tmpmsg));
		if(i == pkg_len / 16)
			h = pkg_len % 16;
		else
			h = 16;
		sprintf(tmpmsg, "%d|%02d-", getpid(), i + 1);
		for(j=0; j<h; j++)
		{
			memset(chl, 0, sizeof(chl));
			sprintf(chl, "%02x ", str[i*16 + j]);
			strcat(tmpmsg, chl);
		}	
		for(j = 0; j < (16 - h); j ++)
		{
			strcat(tmpmsg, "   ");
		}	
		
		for(j = 0; j < h; j ++)
		{
			memset(chl, 0, sizeof(chl));
			if( isprint( str[i*16 + j] ) || str[i*16 + j] > 128)
				sprintf(chl, "%c", str[i*16 + j]);
			else
				sprintf(chl, "%c", '.');
			strcat(tmpmsg, chl);
		}	
		logbasemsg(filepfull, "%s\n", tmpmsg);
	}
	
		WriteLog(ERR_NOW,INFO,"==%d==", getpid());
		return 0;
}


int WriteHexLog(int iLogLvl, char *pcSourceName, int iLine, unsigned char * str, int iLen)
{
	char tmpmsg[30960];
	char sLine[256];
	struct timeval evt_timestamp;
	struct tm *tm;
	int iMsgLen = 0;
	int i = 0, j = 0, h = 0;
	int iHexLine = 0;
	int pkg_len= 0;
	char caFileName[FILENAME_LEN];
	
	if( iLogLvl > pbgetlog() )
	{
		return 0;
	}
	if( (str == NULL) || (iLen == 0) )	return 0;
	
	/*直接打印日志*/
	if(iUseNewLog != 1)
	{
		WriteHexLogNow(iLogLvl, pcSourceName, iLine, str, iLen);
		return 0;
	}
		
	
	memset(caFileName, 0x00,  sizeof(caFileName));
	GetLogFile(caFileName);
	
	memset(&evt_timestamp, 0x00, sizeof(evt_timestamp));
	gettimeofday(&evt_timestamp, NULL);
	tm = (struct tm *)localtime(&evt_timestamp.tv_sec);
	
	memset(sLine, 0x00,  sizeof(sLine));
	snprintf(sLine, sizeof(sLine), "%d|%02d%02d%02d.%03ld|%s:%d-",
					getpid(), tm->tm_hour, tm->tm_min, tm->tm_sec,
				   evt_timestamp.tv_usec/1000, pcSourceName, iLine);

	memset(tmpmsg, 0x00, sizeof(tmpmsg));
	snprintf(tmpmsg, sizeof(tmpmsg), "%s==%8d==\n", sLine, getpid());
	iMsgLen = strlen(tmpmsg);
	
	snprintf(tmpmsg + iMsgLen, sizeof(tmpmsg) - iMsgLen, "%d|DataLen=%d\n", getpid(), iLen);

	iMsgLen = strlen(tmpmsg);
	
	/*AddLogNode*/
	if(AddLogNode(iLogLvl, caFileName, tmpmsg, iMsgLen) != 0)
	{

		CommitLog(-1);
		logbasemsg(caFileName, "%s\n", tmpmsg);
	}
	
	/*拼写HEX数据*/
	pkg_len = iLen;
	iHexLine = pkg_len / 16;
	if(pkg_len % 16 >0)
	{
		/*HEX日志长度除16取余数大于0，则需要多输出一行*/
		iHexLine += 1;
	}
	
	for(i = 0; i < iHexLine; i ++)
	{

		iMsgLen = 0;
		memset(tmpmsg, 0x00, sizeof(tmpmsg));
		
		if(i == pkg_len /16)
			h = pkg_len % 16;
		else
			h = 16;
		
		snprintf(tmpmsg + iMsgLen, sizeof(tmpmsg) - iMsgLen, "%d|02%d-", getpid(), i + 1);
		iMsgLen = strlen(tmpmsg);
		
		for(j = 0; j < h; j ++)
		{
			snprintf(tmpmsg + iMsgLen, sizeof(tmpmsg) - iMsgLen, "02%x ", str[i*16 + j]);
			iMsgLen = strlen(tmpmsg);
		}	
		for(j = 0; j < (16 - h); j ++)
		{
			snprintf(tmpmsg + iMsgLen, sizeof(tmpmsg) - iMsgLen, "   ");
			iMsgLen = strlen(tmpmsg);
		}	
		
		for(j = 0; j < h; j ++)
		{
			if( isprint( str[i*16 + j] ) || str[i*16 + j] > 128)
				tmpmsg[iMsgLen++] = str[i*16 + j];
			else
				tmpmsg[iMsgLen++] = '.';
		}	
		tmpmsg[iMsgLen++]= '\n';
		
		/*AddLogNode	*/
		if(AddLogNode(iLogLvl, caFileName, tmpmsg, iMsgLen) != 0)
		{
			CommitLog(-1);
			logbasemsg(caFileName, "%s\n", tmpmsg);
		}
	}

	memset(tmpmsg, 0x00, sizeof(tmpmsg));
	sprintf(tmpmsg, "%s==%8d==\n", sLine, getpid());
	iMsgLen = strlen(tmpmsg);
	
	/*AddLogNode*/
	if(AddLogNode(iLogLvl, caFileName, tmpmsg, iMsgLen) != 0)
	{
		CommitLog(-1);
		logbasemsg(caFileName, "%s\n", tmpmsg);
	}
	
	return 0;
}

int AddLogNode(int iLvl, char *pcFileName, char *pcMsg, int iMsgLen)
{
	struct LogNode stLogNode;
	struct LogNode *pLogNode = NULL;
	int iFileNo = 0;
	int i = 0;
	char caFileName[FILENAME_LEN];
	int iIsFull = 0;
printf("cominto AddLogNode\n");	
	memset(caFileName, 0x00, sizeof(caFileName));
	/*输入检查*/
	if(pcMsg == NULL)
		return 0;
	
	if(iMsgLen == 0)
		iMsgLen = strlen(pcMsg);
	
	if(pcFileName == NULL)
		snprintf(caFileName, sizeof(caFileName), "%s/log/log", GetEnv("HOME"));
	else
		snprintf(caFileName, sizeof(caFileName), "%s", pcFileName);
printf("AddLogNode step 1\n");	
	for(i = 0; i<PER_MAX_LOG_FILE; i++)
	{
printf("AddLogNode step 2\n");	
		if(strcmp(caFileName, caPerLogFile[i] )== 0)
		{
			iFileNo = i;
			break;
		}
printf("AddLogNode step 3\n");	
		if(caPerLogFile[i][0] == '\0')
		{
			iFileNo = i;
			snprintf(caPerLogFile[i], sizeof(caPerLogFile[i]), "%s", caFileName);
			break;
		}
	}
printf("AddLogNode step 4\n");		
	/*初始化日志节点信息*/
	memset(&stLogNode, 0x00, sizeof(struct LogNode));
	if(pbgetlog() == 0)
		stLogNode.iLogLvl = iLvl;
	else
		stLogNode.iLogLvl = 1;
		stLogNode.iLogLen = 0;
		stLogNode.iFileNo = iFileNo;
		stLogNode.next 	  = NULL;
		
		do{
			pLogNode = WriteLogNode(&stLogNode);
			if(pLogNode == NULL)
				return -1;
			else
			{
				if(stLogHead.pNode == NULL)
				{
					stLogHead.pNode = pLogNode;
					pEndNode = pLogNode;
				}
				else{
					pEndNode->next = pLogNode;
					pEndNode = pLogNode;
				}
			}
			
			iIsFull = 0;
			pLogNode->pLogInfo = WriteLogInfo(pcMsg, iMsgLen, &iIsFull);
			if(pLogNode->pLogInfo == NULL)
			{
				if(iIsFull == 1)
					continue;
				else
					return -1;
			}		
		}while(iIsFull);
		
		pLogNode->iLogLen = iMsgLen;
printf("AddLogNode step 5\n");		
		return 0;
}

struct LogNode * WriteLogNode(struct LogNode *pNode)
{
	char *pRet = NULL;
	int iSize  = 0;
	
	if(pNode == NULL) return NULL;
	if(pMemHead == NULL) pMemHead = caLogBuff;
	
	iSize = sizeof(struct LogNode);
	
	if(lUsedSize + iSize > LOG_BUFF_SIZE)
		CommitLog(-1);
	
	pRet = pMemHead +lUsedSize;
	memcpy(pRet, pNode, iSize);
	lUsedSize += iSize;
	return (struct LogNode*)pRet;
}

char * WriteLogInfo(char *pcInfo, int iSize, int *ipFlag)
{
	char *pRet = NULL;
	
	if(pcInfo == NULL) return NULL;
	if(pMemHead == NULL) return NULL;
	if(lUsedSize == 0) return NULL;
	if( iSize == 0)
		iSize = strlen(pcInfo);
	
	if(lUsedSize + iSize + 1 > LOG_BUFF_SIZE)
	{
		CommitLog(0);
		*ipFlag = 1;
		return NULL;
	}
	
	pRet = pMemHead +lUsedSize;
	memcpy(pRet, pcInfo, iSize);
	lUsedSize += iSize + 1;
	return pRet;
}

/*CommitLog*/


int logbasemsg(char *filename, char *fmt, ...)
{
	va_list args;
	char tmpMsg[10240];
	char tmpStr[10240];
	char tmpfname[256];
	char filefull[MAXNAMELEN];
	FILE *fp = NULL;
	
	memset(tmpMsg, 	 0x00, sizeof(tmpMsg));
	memset(tmpStr, 	 0x00, sizeof(tmpStr));
	memset(tmpfname, 0x00, sizeof(tmpfname));
	memset(filefull, 0x00, sizeof(filefull));
	
	va_start (args, fmt);
	vsnprintf(tmpMsg, sizeof(tmpMsg) - 1, fmt, args);
	va_end(args);
	
	
	GetLogFile(tmpfname);
	if(BuffMode && strcmp( tmpfname ,filename) == 0)
	{
		return 0;
	}
	
	if(strstr(filename, ".log") != NULL)
		sprintf(filefull,"%s", filename);
	else
	{
		sprintf(filefull,"%s", filename);
	}

	fp = fopen(filefull, "a");
	
	if((fp == NULL))
	{
		fprintf(stderr, "pid=[%d], Log File %s open Error! ERRNO [%d]\n", getpid(), filefull, errno);
		return -1;
	}
	ChkFileSize(fp, filefull);
	fprintf(fp, "%s", tmpMsg);
	if(fp != stderr || fp != NULL)
		fclose(fp);
	return 0;
}

int ChkFileSize(FILE* fp, char *filename)
{
	int i;
	int ret = -1;
	unsigned long long size;
	struct timeval evt_timestamp;
	struct flock lock;
	struct stat fs;
	struct tm *ftm;
	char temp[64];
	char tmpfilename[FILENAME_LEN];
	char newfilename[FILENAME_LEN];
	
	memset(&fs, 0, sizeof(struct stat));
	size = LogFileSize*1024*1024ULL;
	
	fstat(fileno(fp), &fs);
	
	if(fs.st_size == 0 || (unsigned long long)(fs.st_size)<size)
		return 0;
	
	memset(temp, 0, 64);
	memset(tmpfilename, 0, FILENAME_LEN);
	memset(newfilename, 0, FILENAME_LEN);
	
	for(i = 0; i<FILENAME_LEN; i++)
	{
		if(filename[i] == '.')
			break;
	}
	memcpy(tmpfilename, filename, i);
	strcpy(temp,filename+i);
	
	memset(&evt_timestamp, 0x00, sizeof(evt_timestamp));
	gettimeofday(&evt_timestamp, NULL);
	ftm = (struct tm *)localtime(&evt_timestamp.tv_sec);
	
	sprintf(newfilename, "%s_%02d%02d_%02d%02d%02d%s", tmpfilename, ftm->tm_mon+1, ftm->tm_mday,ftm->tm_hour,ftm->tm_min,ftm->tm_sec,temp);
	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	
	if(fcntl(fileno(fp), F_SETLK, &lock) == 0)
	{
		ret = access(newfilename, 0);
		if(ret != 0)
		{
			if(errno == ENOENT)
			{
				if(rename(filename, newfilename) == -1)
					fprintf(stderr, "Rename [%s] To [%s] Error! ERRNO[%d]\n", filename, newfilename, errno);
			}
			else
			{
				fprintf(stderr, "ACCESS [%s] ERROR! ERRNO[%d]\n", newfilename ,errno);
			}
		}
		lock.l_type = F_UNLCK;
		fcntl(fileno(fp), F_SETLK, &lock);
	}
	return 0;
}
void CommitLog(int iInfo)
{
	FILE *logfp = NULL;
	char caLogFileName[MAXNAMELEN];
	FILE *logfptmp = NULL;
	char tmpFileName[MAXNAMELEN];
	struct LogNode *pLogNode = NULL;
	int iValidLvl = 0;
	int iIsError = 0;
	int nofullflag = 0;
	
	if(stLogHead.pNode == NULL)
		return;
	
	memset(caLogFileName, 0x00, sizeof(caLogFileName));
	memset(tmpFileName,   0x00, sizeof(tmpFileName));
	
	if(iInfo != 0 || (stLogHead.iLogStatus == 1))
	{
		iValidLvl = stLogHead.iLogError;
		iIsError = 1;
	}
	else
		iValidLvl = stLogHead.iLogNormal;
	
	/*写入文件*/
	pLogNode = stLogHead.pNode;
	while(pLogNode != NULL)
	{
		if( (pLogNode->iLogLen == 0) || (pLogNode->pLogInfo == NULL) )
		{
			pLogNode = pLogNode->next;
			continue;
		}
		
		if(memcmp(caLogFileName, caPerLogFile[pLogNode->iFileNo], strlen(caPerLogFile[pLogNode->iFileNo])) != 0)
		{
			if( (logfp != NULL) && (logfp != stderr))
			{
				fclose(logfp);
				logfp = NULL;
			}
			if(logfptmp != NULL && logfptmp != stderr)
			{
				fclose(logfptmp);
				logfptmp = NULL;
			}
			
			nofullflag = -1;
			memset(caLogFileName, 0x00, sizeof(caLogFileName));
			memset(tmpFileName,   0x00, sizeof(tmpFileName));
			if(iIsError  == 1)
			{
				if(strcmp(caPerLogFile[pLogNode->iFileNo], "/dev/null") == 0)
					sprintf(tmpFileName, "/dev/null");
				else
					snprintf(tmpFileName, sizeof(tmpFileName), "%s.log.E",caPerLogFile[pLogNode->iFileNo]);
			}
			else
			{
				if(strstr(caPerLogFile[pLogNode->iFileNo], "InitMain") || strstr(caPerLogFile[pLogNode->iFileNo], "tpsvrinit"))
					nofullflag = 0;
				else
				{
					nofullflag = -1;
				if(stLogHead.iLogNormal != 0)
					sprintf(tmpFileName, "%s.full.log", caPerLogFile[pLogNode->iFileNo]);
				}
			}
			
			/*xxx.log文件都会生成*/
			snprintf(caLogFileName, sizeof(caLogFileName), "%s.log", caPerLogFile[pLogNode->iFileNo]);
			logfp = fopen(caLogFileName, NULL);
			if(logfp == NULL)
			{
				fprintf(stderr,"pid=[%d], Log File %s open Error! ERRNO [%d]\n", getpid(), caLogFileName, errno);
				break;
			}
			
			ChkFileSize(logfp, caLogFileName);
			if(strlen(tmpFileName) != 0)
			{
				logfptmp = fopen(tmpFileName, "a");
				if(logfptmp == NULL)
				{
					fprintf(stderr,"pid=[%d], Log File %s open Error! ERRNO [%d]\n", getpid(), tmpFileName, errno);
					break;
				}
				ChkFileSize(logfptmp, tmpFileName);
			}
		}
			if(((iValidLvl >= pLogNode->iLogLvl) && ((pLogNode->iLogLvl) <= 0)) || !nofullflag) 
			{
				fprintf(logfp, "%s", pLogNode->pLogInfo);
				if(pLogNode->pLogInfo[pLogNode->iLogLen - 1] != '\n')
					fprintf(logfp, "\n");
			}
			
			if(strlen(tmpFileName) != 0)
			{
				if(iIsError == 1)
				{
					if(iValidLvl > pLogNode->iLogLvl)
					{
						fprintf(logfptmp, "%s", pLogNode->pLogInfo);
						if(( pLogNode->pLogInfo[pLogNode->iLogLen - 1]) != '\n')
							fprintf(logfptmp,"\n");
					}
				}
				else
				{
					if( (iValidLvl >= pLogNode->iLogLvl) && stLogHead.iLogNormal != 0)
					{
						fprintf(logfptmp, "%s", pLogNode->pLogInfo);
						if(pLogNode->pLogInfo[pLogNode->iLogLen - 1] != '\n')
							fprintf(logfptmp, "\n");
					}
				}
			}
			pLogNode = pLogNode->next;		
	}
	if(logfp != stderr && logfp != NULL)
		fclose(logfp);
	if(logfptmp != stderr && logfptmp != NULL)
		fclose(logfptmp);
	
	ClearLogBuff();
	return;
}

void ClearLogBuff(void)
{
	memset(caLogBuff, 0x00, sizeof(caLogBuff));
	
	lUsedSize = 0;
	stLogHead.pNode = NULL;
	pEndNode = NULL;
	return;
}
