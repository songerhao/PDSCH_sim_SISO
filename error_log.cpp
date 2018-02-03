#include "error_log.h"
#include <io.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
int loger(char* err_msg)
{
		/*
	错误的记录格式为时间加 错误信息
	*/
	char* error_log_file_path="log/error.log";
	time_t rawtime;
	struct tm* log_time;
	time(&rawtime);
	log_time=localtime(&rawtime);
	FILE* fp_log;
	if (access(error_log_file_path,0))
	{
		fp_log=fopen(error_log_file_path,"w");
		fprintf(fp_log,"Log File first created\t---->%s",asctime(log_time));
	}
	else
	{
		fp_log=fopen(error_log_file_path,"a");
		if (!fp_log)
		{
			printf("fail to open log file,ignored!\t---->%s",asctime(log_time));
			return -1;
		}
	}
	fprintf(fp_log,"%s\t---->%s",err_msg,asctime(log_time));
	fclose(fp_log);
	return 0;

}
