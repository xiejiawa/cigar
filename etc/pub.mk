#
#           公共 Makefile 子文件
#---------------------------------------------------
#   DDEFINEPART1: 宏定义开关，环境变量，编译控制
#   DDEFINEPART2: 宏定义开关，环境变量，编译控制
#   说明：具体配置参考...
#---------------------------------------------------

BINDIR    = $(HOME)/cigar/bin
LIBDIR    = $(HOME)/cigar/lib
INCDIR    = $(HOME)/cigar/include
INCLUDE   = /usr/local/include
MY_INCDIR = /usr/include

#在此加入CC编译参数
CC 	  = gcc -Wall -Wextra -Werror -Wundef -fPIC -m64 -D X86_PCAP -lstdc++
C+ 	  = g++ -Wall -Wextra -Werror -Wundef -fPIC -m64 -D X86_PCAP -lstdc++

CCARGS	  = -g  
INCFLAGS  = -I$(INCDIR) -I$(MY_INCDIR) -I$(INCLUDE)
LIBFLAGS  = -L/usr/lib -L$(LIBDIR)

CCFLAGS   = $(CCARGS) $(INCFLAGS) $(LIBFLAGS)

#编译成可执行文件
CC_BUILD_NEW = $(CC) $(CCFLAGS) -o $(@) $(?) 
C+_BUILD_NEW = $(C+) $(CCFLAGS) -o $(@) $(?) 

#把.c预处理成.o文件
CC_BUILD_C   = $(CC) -c $(CCFLAGS) -o $(@) $<
C+_BUILD_C   = $(C+) -c $(CCFLAGS) -o $(@) $<


#在此加入ar[SLIB]参数
AR          = ar
ARARGS_NEW  = ruv
ARFLAGS_NEW = $(ARARGS_NEW)
#把.o文件链接成库文件.a
AR_BUILD = $(AR) $(ARFLAGS_NEW) $(@) $(?F)

#在此加入ar[DLIB]参数
#把.o文件链接成库文件.a
SO_BUILD = $(CC) $(CCARGS) -shared -o $(@) $(?F)

#特殊目标.SUFFIXES
.SUFFIXES: .pc .c .o .h .cpp

.c.o:
	$(CC_BUILD_C)

.cpp.o:
	$(C+_BUILD_C)
