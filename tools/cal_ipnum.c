#include <time.h>
#include <errno.h>
//#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


int main()
{
	int IP_1 = 1;
	int IP_2 = 0;
	int IP_3 = 0;
	int IP_4 = 0;
	long long unsigned int count = 0;
	FILE *fp;
	char rule_str[1024];
	char file_name[1024];
	memset(rule_str, 0x00, 1024);
	memset(file_name, 0x00, 1024);
    sprintf(file_name, "ip_co.txt");
	fp = fopen(file_name, "w+" );
	if( fp == NULL ) {
		printf( "open file error!!\n" );
		return -1;
	}
	setbuf( fp, (char*)NULL );


	for(IP_1=2; IP_1 < 256; IP_1++) {
		for(IP_2=0; IP_2 < 256; IP_2++) {
		    	for(IP_3=0; IP_3 < 256; IP_3++) {
					for(IP_4=0; IP_4 < 256; IP_4++) {
						sprintf(rule_str,"%d.%d.%d.%d\n", IP_1, IP_2, IP_3, IP_4);
						//if(IP_1 == 16 && IP_2 == 0 && IP_3 == 250 && IP_4 == 250) {
							if(count >=8000000 ) {
							count++;
							fprintf(fp, "%s", rule_str);
							goto stop_all_loop;
						}
						fprintf(fp, "%s", rule_str);
						count++;
					}
				}
			}
	}
	
stop_all_loop:
	printf("ip_count=[%llu]\n", count);
	printf("%d.%d.%d.%d\n", IP_1, IP_2, IP_3, IP_4);
	if( fp != NULL )
		fclose(fp);
	return 0;
}
				

