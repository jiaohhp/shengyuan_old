TARGETS=./src/config/src/ ./src/hard/src ./src/media/src ./src/net/src ./src/public/src ./src/property/src ./src/sys_mag/src

OBJDIR=./obj/ 
TARGET=./bin/IpcServer
#TARGET=/nfsroot/IpcServer

TODOTARGET=./bin/todo

MAINSRC=./main.c
MAINOBJ=./obj/main.o

TODOSRC=./mytodo.c
TODOOBJ=./obj/mytodo.o

INC=-I $(PWD)/include/hi_head/ -I $(PWD)/include/bh_head/      -I$(PWD)/src/config/include/     -I $(PWD)/src/hard/include/     -I $(PWD)/src/media/include/      -I $(PWD)/src/net/include/      -I $(PWD)/src/public/include/     -I $(PWD)/src/property/include/ -I $(PWD)/src/sys_mag/include/

 
LIBS+= -L./obj/ -lmedia -lhard -lnet -lconfig  -lproperty -lsys_mag -lpublic
LIBS+= -L./lib -lmpi -lisp -laec -lresampler -lanr -lVoiceEngine
LIBS+= -lpthread -lm

MAKE=make -C
MAKECLEAN=make clean -C

CFLAGS+= -Werror -Wall -g -mlittle-endian
#CFLAGS+= -Werror -Wall -O2 -mlittle-endian

#hisi compile not support this option
#CFLAGS += -Wno-error=unused-but-set-variable

CROSS ?= arm-hisiv100-linux-
CC=$(CROSS)gcc
AR=$(CROSS)ar
STRIP=$(CROSS)strip

export CFLAGS CC AR INC

all:$(OBJDIR) LIBS $(MAINOBJ) $(TODOOBJ)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~"   	
	@echo "|MAKE EXECUTE FILE|"   	
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	$(CC) $(CFLAGS) -o $(TARGET) $(MAINOBJ) $(LIBS)
	$(CC) $(CFLAGS) -o $(TODOTARGET) $(TODOOBJ) $(LIBS)
	@echo "******************************************************"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"   	
	@echo "|BUILDING SUCCESSFUL|"   	
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" 
	@echo "STRIP THE CONFIG_SERVER|" 
	@$(STRIP) $(TARGET)
	@$(STRIP) $(TODOTARGET)

$(OBJDIR):
	@mkdir -p $@
	@echo "******************************************************"
	
$(MAINOBJ):$(MAINSRC)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~"   	
	@echo "|COMPILE MAIN OBJ|"   	
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~" 
	$(CC) -o $(MAINOBJ) -c $< $(CFLAGS) $(INC) 
	@echo "******************************************************"
	
$(TODOOBJ):$(TODOSRC)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~"   	
	@echo "|COMPILE MAIN OBJ|"   	
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~" 
	$(CC) -o $(TODOOBJ) -c $< $(CFLAGS) $(INC) 
	@echo "******************************************************"
	
LIBS:
	for i in $(TARGETS); do $(MAKE) $$i; if [ $$? -ne 0 ]; then exit 254;  fi; done
	
clean:
	@for i in $(TARGETS); do $(MAKECLEAN) $$i; done
	-rm -Rf $(OBJDIR)
	-rm $(TARGET)
	-rm $(TODOTARGET)
	@echo "******************************************************"

.PHONY:clean all
