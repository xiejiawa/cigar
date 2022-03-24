include $(HOME)/cigar/etc/pub.mk

TARGET = rule_crate
LINK_LIB = $(LIBDIR)/errlog.a 

rule_crate:crate_rule.o
	@echo 
	@echo "正在编译...$@"
	$(CC_BUILD_NEW) $(LINK_LIB)
	@rmpre	
	@echo "done"
	@echo 

all:$(TARGET)
