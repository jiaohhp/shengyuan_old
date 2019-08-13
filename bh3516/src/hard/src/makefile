LIB=libhard.a
LIBPATH=../../../obj/
SRC:=$(wildcard *.c)
OBJS:=$(SRC:%.c=%.o)
all:$(OBJS)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "| MAKE APP LIB |"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	$(AR) -rc -o $(LIBPATH)$(LIB) $(OBJS)
	@echo "******************************************************"
$(OBJS): %.o : %.c
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "| COMPILE $< |"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	$(CC) -DHI_GPIO_I2C -c $< $(CFLAGS) $(INC)
	@echo "******************************************************"
clean:
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "| CLEANUP APP MODUL |"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	-rm -f *.o *.a
	@echo "******************************************************"
.PHONY:clean
