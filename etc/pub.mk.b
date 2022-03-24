ARCH=x86

CC        = gcc 
OPT       = -g
CFLAGS    = -Wall $(OPT) -Wextra -m64 -D X86_PCAP -lstdc++
THREAD    = -lpthread

#把.c预处理成.o文件
CC_BUILD_C   = $(CC) -c $(CFLAGS) -o $(@) $<

#编译成可执行文件
CC_BUILD_NEW = $(CC) $(CFLAGS) $(THREAD) $(INCLUDE) -o $(@) $(?)

#把.o文件连接到库.a
AR          = ar
ARARGS_NEW  =
ARFLAGS_NEW = $(ARFLAGS) $(ARARGS_NEW)
AR_BUILD = $(AR) $(ARFLAGS_NEW) $(@) $(?F)

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

$(OBJ):%.o:%.c
	$(CC_BUILD_C) 


	
