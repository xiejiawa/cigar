include $(HOME)/cigar/etc/pub.mk

SLIB = $(HOME)/cigar/lib/errlog.a
DLIB = $(HOME)/cigar/lib/errlog.so

all:$(SLIB) $(DLIB)

$(SLIB) : errlog.o 
	@echo
	@echo "正在建库...$@"
	$(AR_BUILD)
#	@rmpre	
	@echo "done"
	@echo 

$(DLIB) : errlog.o 
	@echo
	@echo "正在建库...$@"
	$(SO_BUILD)
#	@rmpre	
	@echo "done"
	@echo 

clean:
	@echo
	@rmpre
	@rm -fr $(SLIB) $(DLIB)
