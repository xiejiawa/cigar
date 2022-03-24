include $(HOME)/cigar/etc/pub.mk

TARGET = pcap_make
LINK_LIB = 
#$(LIBDIR)/errlog.a 

pcap_make:pcap_make.o
	@echo 
	@echo "正在编译...$@"
	$(CC_BUILD_NEW) $(LINK_LIB)
	@rmpre	
	@echo "done"
	@echo 


cal_ipnum:cal_ipnum.o
	@echo 
	@echo "正在编译...$@"
	$(CC_BUILD_NEW) $(LINK_LIB)
	@rmpre	
	@echo "done"
	@echo 

all:$(TARGET)
