#ARCH=ppc
#ARCH=ppc64
ARCH=x86

ifeq ($(ARCH), ppc)
#CC=/opt/ppc/powerpc-none-linux-gnuspe/bin/powerpc-none-linux-gnuspe-gcc
#CC=/opt/platform_rel/freescale/mpc8314/dist_b2000_v20_2012-09-07/powerpc-e300c3-linux-gnu/bin/powerpc-e300c3-linux-gnu-gcc
CC=/opt/platform_rel/freescale/p2020/sm2020_v11_2014-06-02/powerpc-none-linux-gnuspe/bin/powerpc-none-linux-gnuspe-gcc
CCC=/opt/platform_rel/freescale/mpc8314/dist_b2000_v20_2012-09-07/powerpc-e300c3-linux-gnu/bin/powerpc-e300c3-linux-gnu-g++
MGMT_API =  ../lib/ppc32/libeb_mgmt_api.a
endif

ifeq ($(ARCH), ppc64)
#PATH_DIR=/opt/ppc64/fsl-qoriq/1.9/sysroots/x86_64-fslsdk-linux/usr/bin/powerpc64-fsl-linux/
#CC=${PATH_DIR}/powerpc64-fsl-linux-gcc
CC=/opt/platform_rel/fsl-qoriq/1.9/sysroots/x86_64-fslsdk-linux/usr/bin/powerpc64-fsl-linux/powerpc64-fsl-linux-gcc
CCC=/opt/platform_rel/fsl-qoriq/1.9/sysroots/x86_64-fslsdk-linux/usr/bin/powerpc64-fsl-linux/powerpc64-fsl-linux-g++
#EBPOOL = ../extern_modules/eb_pool/lib/ppc64/libebpool.a
MGMT_API = ../extern_modules/eb_xml_lib/ppc64/libmxml.a ../lib/ppc64/libeb_mgmt_api.a ../lib/ppc64/libeb_rpc.a ../lib/ppc64/libeb_sdh_analyse_client.a
#MGMT_API = ../lib/libmgmt_api.a ../lib/libeb_rpc.a ../lib/ppc64/libeb_sdh_analyse_client.a ../extern_modules/eb_xml_lib/ppc64/libmxml.a
endif

ifeq ($(ARCH), x86)
CC=gcc 
#EBPOOL = ../extern_modules/eb_pool/lib/x86/libebpool.a
#MGMT_API = ../lib/x86/libeb_mgmt_api.a ../lib/x86/libeb_rpc.a ../lib/x86/libeb_sdh_analyse_client.a ../extern_modules/eb_xml_lib/x86/libmxml.a
MGMT_API = ../lib/x86/libeb_mgmt_api.a 
endif


OPT = -g
#OPT = -O3
CFLAGS = -Wall $(OPT) -D X86_PCAP -lstdc++
THREAD = -lpthread

#INCLUDE = -I../extern_modules/include \
		  -I../extern_modules/eb_pool/include\
		  -I../extern_modules/eb_xml_lib/include \
		  -I../include
		  
INSTALL_PATH = /home/zk
	
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

TARGET =thread_mutex

	
all:$(TARGET)

thread_mutex:thread_mutex.o #${MGMT_API}
	$(CC) $(CFLAGS) $(THREAD) $(INCLUDE) $^ -o $@
	rm -rf *.o	

$(OBJ):%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $^ -o $@

.PHONY:clean
clean:
	rm -rf *.o $(TARGET) $(OBJS)

	
