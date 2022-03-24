#include <time.h>
#include <errno.h>
//#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

//时间戳
struct time_val
{
    uint32_t tv_sec;   /* 4B 时间戳高位，精确到seconds，这是Unix时间戳。捕获数据包的时间一般是根据这个值 */
    uint32_t tv_usec;  /* 4B 时间戳低位，能够精确到microseconds */
};

/* pcap文件头 24B */
typedef struct Pcap_file_header {
	uint32_t magic;				/* 4B 0xa1b2c3d4表示是大端模式，0xd4c3b2a1表示小端模式*/
    uint16_t version_major;		/* 2B 当前文件的主要版本号，一般为0x0200 */
    uint16_t version_minor;		/* 2B 当前文件的次要版本号，一般为0x0400 */
    uint32_t thiszone;      	/* 4B 当地的标准事件，如果用的是GMT则全零，一般全零 gmt to local correction */
    uint32_t sigfigs;       	/* 4B 时间戳的精度，一般为全零 accuracy of timestamps */
    uint32_t snaplen;       	/* 4B 最大的存储长度，设置所抓获的数据包的最大长度，如果所有数据包都要抓获，将值设置为65535 max length saved portion of each pkt */
    uint32_t linktype;      	/* 4B 链路类型。解析数据包首先要判断它的LinkType，所以这个值很重要。一般的值为1，即以太网  data link type (LINKTYPE_*) */
}pcap_file_header;

/* pcap数据包头结构体 16B */
typedef struct pcap_pkthdr
{
    struct time_val ts;  /* 8B time stamp */
    uint32_t caplen;     /* 4B 当前数据区的长度，即抓取到的数据帧长度，由此可以得到下一个数据帧的位置。length of portion present */
    uint32_t len;        /* 4B 离线数据长度，网路中实际数据帧的长度，一般不大于Caplen，多数情况下和Caplen值一样 length this packet (off wire) */
}pcap_pkthdr_header;

/*数据帧头：数据链路层的头部 14B*/
typedef struct FameHead
{
	uint8_t  Dmac[6];
	uint8_t  Smac[6];
	uint16_t FrameType;
}pcap_frame_header;

/*IP数据报头 20B*/
typedef struct st_ip_head{
	uint8_t  Ver_HLen;			/*版本+报头长度*/
	uint8_t  TOS;				/*服务类型*/
	uint16_t TotalLen;			/*总长度*/
	uint16_t identification;	/*标识*/
	uint16_t Flag_Segment;		/*标志+分片偏移*/
	uint8_t  ttl;				/*生存周期*/
	uint8_t  protocol;			/*协议类型*/
	uint16_t checksum;			/*头部校验和*/
	uint32_t s_ip;				/*源IP地址*/
	uint32_t d_ip;				/*目的IP地址*/
}pcap_ip_header;

/*TCP数据报头 20B*/
typedef struct st_tcp_head{
	uint16_t s_port;
	uint16_t d_port;
	uint32_t seq;
	uint32_t ack;
	uint8_t head_len;
	uint8_t flags; 	
	/*8 bit
	| 0 |    1   |   2    |   3   |  4   |  5  |  6  |  7  |
	|cwr|ecn-echo| urgent |  ack  | push | rst | syn | fin |*/
	uint16_t window;
	uint16_t checksum;
	uint16_t urgentPointer;
}pcap_tcp_header;

uint16_t GetIpCheckSum( char *ptr, int size)
{
	int cksum = 0;
	int index = 0;
	
	*(ptr + 10) = 0;
	*(ptr + 11) = 0;
 
	if(size % 2 != 0)
		return 0;
	
	while(index < size)
	{       
		cksum += *(ptr + index + 1);
		//printf("cksum[%d][0x%x]\n", cksum,cksum);
		cksum += *(ptr + index) << 8;
		//printf("cksum[%d][0x%x]\n", cksum,cksum);
		//cksum += ((*(ptr + index))&0xff00) | ((*(ptr + index + 1))&0x00ff); 
 		
		index += 2;
	}
	//printf("*cksum[%d][0x%x]\n", cksum,cksum);
 
	while(cksum > 0xffff)
	{
		cksum = (cksum >> 16) + (cksum & 0xffff);
		//printf("-cksum[%d][0x%x]\n", cksum,cksum);
	}
	//printf("-------cksum[%d][0x%x]\n", cksum,cksum);
	//printf("-------cksum[%d][0x%x]\n", cksum,~cksum);
	return ~cksum;
}


int main() {
	FILE * fp 		= NULL;
	pcap_file_header 	head_file;
	pcap_pkthdr_header  head_cap;
	pcap_ip_header  	head_ip;
	pcap_tcp_header		head_tcp;
	pcap_frame_header	head_frame;
	int len_pcap_head 	= sizeof(pcap_file_header);
	int len_pcap 		= sizeof(pcap_pkthdr_header);
	int len_ip_head 	= sizeof(pcap_ip_header);
	int len_tcp_head 	= sizeof(pcap_tcp_header);
	int len_pcap_frame	= sizeof(pcap_frame_header);
	int n = 0;
	time_t time_now = time(NULL);
	int start_payload = 0;
	int pload_len = 0;
	int ret = 0;
	char path_pcap[32] = {0};
	char file_pcap[64] = {0};
	char tmp_str[256] = {0};
	
	uint32_t tmp_uint = 0;
	uint32_t s_ip = 0;
	uint32_t d_ip = 0;
	uint32_t s_port = 0;
	uint32_t d_port = 0;
	
	head_file.magic 		= 0xa1b2c3d4;    
    head_file.version_major = 0x0002;    
    head_file.version_minor = 0x0004;    
    head_file.thiszone 		= 0x00000000;    
    head_file.sigfigs 		= 0x00000000;    
    head_file.snaplen 		= 0x0000ffff;    
    head_file.linktype 		= 1;	

	head_frame.Dmac[0]		= 0x10;
	head_frame.Dmac[1]		= 0x11;
	head_frame.Dmac[2]		= 0x12;
	head_frame.Dmac[3]		= 0x13;
	head_frame.Dmac[4]		= 0x14;
	head_frame.Dmac[5]		= 0x15;	
	head_frame.Smac[0]		= 0x06;
	head_frame.Smac[1]		= 0x07;
	head_frame.Smac[2]		= 0x08;
	head_frame.Smac[3]		= 0x09;
	head_frame.Smac[4]		= 0x0a;
	head_frame.Smac[5]		= 0x0b;
	head_frame.FrameType	= 0x0008;

	sprintf(path_pcap, "path_pcap");//, time_now);
	mkdir(path_pcap, 0777);
	
	sprintf(file_pcap, "./%s/test1.pcap", path_pcap);
	fp = fopen(file_pcap, "wb+");
	if(NULL == fp){
		
		sprintf((char*)tmp_str, "fopen '%s' err:", file_pcap);
		perror((char*)tmp_str);
		return -1;
	}
	ret = fwrite(&head_file, 1, len_pcap_head, fp);
	if(ret != len_pcap_head){
		printf("%d:head_cap_file write_err:%d:%d\n", __LINE__, ret, len_pcap_head);
	}
	d_ip = 100*256*256*256 + 1;
	s_port = 80;
	d_port = 10001;
	for(s_ip = 0x01000001;s_ip <= 0x01000001; s_ip++){
		start_payload = 14 + len_ip_head + len_tcp_head;
		pload_len = 256 - start_payload;
		for(n = 0; n < 1;n++){
			head_cap.ts.tv_sec = time_now;
			head_cap.ts.tv_usec = n * 1000 + n + 1;
			head_cap.caplen = 256;
			head_cap.len = 256;
			head_ip.Ver_HLen = 0x45;
			head_ip.TOS = 0x00;
			head_ip.TotalLen = htons(256 - 14);
			head_ip.identification = 0x6400;
			head_ip.Flag_Segment = 0x0;
			head_ip.ttl = 255;
			head_ip.protocol = 6;
			head_ip.checksum = 0;
			head_ip.s_ip = htonl(s_ip);
			head_ip.d_ip = htonl(d_ip);
			head_ip.checksum = GetIpCheckSum( (char*)&head_ip, len_ip_head);
			head_tcp.s_port = htons(s_port);//(1024%256) * 256 + 1024/256;
			head_tcp.d_port = htons(d_port);//(1024%256) * 256 + 1024/256;
			tmp_uint = n*pload_len + 100;
			head_tcp.seq = htonl(tmp_uint);
			head_tcp.ack = htonl(1);
			head_tcp.head_len 	= 0x50;
			head_tcp.flags 		= (1<<4&0xf0)|(0&0xf);
			head_tcp.window 	= 0x10;
			head_tcp.checksum = 0;
			head_tcp.urgentPointer = 0;

			//memset(tmp_str,0,20);
			ret = fwrite(&(head_cap), 1, len_pcap, fp);
			if(ret != len_pcap){
				printf("%d:head_cap write_err:%d:%d\n", __LINE__, ret, len_pcap);
			}
			ret = fwrite(&(head_frame), 1, len_pcap_frame, fp);
			if(ret != 14){
				printf("%d:mac write err:%d:14\n", __LINE__, ret);
			}
			ret = fwrite(&(head_ip), 1, len_ip_head, fp);
			if(ret != len_ip_head){
				printf("%d:head_ip write err:%d:%d\n", __LINE__, ret, len_ip_head);
			}
			ret = fwrite(&(head_tcp), 1, len_tcp_head, fp);
			if(ret != len_tcp_head){
				printf("%d:head_tcp write err:%d:%d\n", __LINE__, ret, len_tcp_head);
			}
			if(0 == n){
				ret = fwrite("TEST-PCAP FILE", 1, 14, fp);
				if(14 != ret){
					printf("%d:HTTP/1.1 write err:%d\n", __LINE__, ret);
				}
				ret = fwrite(tmp_str + 14, 1, pload_len - 14, fp);
				if(ret != (pload_len - 14)){
					printf("%d:pload write err:%d:%d\n", __LINE__, ret, pload_len - 8);
				}
			}else{
				ret = fwrite(tmp_str + 14, 1, pload_len, fp);
				if(ret != pload_len){
					printf("%d:pload write err:%d:%d\n", __LINE__, ret, pload_len);
				}
			}
		}
	}
	fclose(fp);
	
	return 0;
}


