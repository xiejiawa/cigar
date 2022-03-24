/*************************************************************************
      > File Name: basefunc.h
      > Author: 谢佳
      > mail: seamas.x@gmail.com 
      > Created Time: 2020-03-24 10:28:57
      >描述:××××
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include <string.h>

char logfile[128];
#define DEBUG "DEBUG"
#define ERROR "ERROR"
#define INFO  "INFO"

#define SUPERR_LVL  -1,__FILE__,__func__,__LINE__    
#define ERR_LVL     0,__FILE__,__func__,__LINE__   
#define DATALOG_LVL 1,__FILE__,__func__,__LINE__  
#define DATA_LVL    1,__FILE__,__func__,__LINE__ 
#define DEBUG_LVL0  2,__FILE__,__func__,__LINE__    
#define DEBUG_LVL1  3,__FILE__,__func__,__LINE__   
#define DEBUG_TEST  4,__FILE__,__func__,__LINE__  

#define ERR     0,__FILE__,__LINE__
#define ERR_NOW     0,__FILE__,NULL,__LINE__

int LOGL;


extern int getlogLvl();
//void  pbsetlog(int loglvl );
//extern int WriteLog(int loglev, char * filename1, const char * func, int line, char * infolev, char * fmt, ... );
extern int WriteLog(int loglev, char * filename1, const char * func, int line, char * fmt, ... );
extern int WriteHexLog(int, char *, int, unsigned char *, int);

//#define DBUG(...) WriteLog(ERR_LVL, DEBUG, __VA_ARGS__)
#define DBUG(...) WriteLog(ERR_LVL,  __VA_ARGS__)
//#define DBG(char* infolev, ...) WriteLog(ERR_LVL, char* infolev, __VA_ARGS__)
