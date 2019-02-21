
CC=sdcc
ROOT_PROJ=$(strip $(CURDIR))
INCLUDE_DIRS=-I$(ROOT_PROJ) -I$(ROOT_PROJ)/misc/ -I$(ROOT_PROJ)/mode2T/

TARGET:=timer-led
MAIN_C=main

OUT_FOLDER:=build
OBJS=
C_SRCS=

C_SRCS+=$(MAIN_C).c
OBJS+=$(MAIN_C).rel

CFLAGS= -DSTM8S103  --all-callee-saves --debug --stack-auto --fverbose-asm  --float-reent --no-peep

-include ./misc/misc.mk

.PHONY: all flash

all: $(TARGET).ihx Makefile

$(TARGET).ihx:  $(OBJS)
	@echo ' --------------------------------------- '
	@echo 'Final compile and link $@'
	$(CC) -o $(TARGET).ihx $(CFLAGS)  -mstm8 $(INCLUDE_DIRS) --out-fmt-ihx $(OBJS)
	$(CC) -o $(TARGET).elf $(CFLAGS)  -mstm8 $(INCLUDE_DIRS) --out-fmt-elf $(OBJS)
	stm8-size  $(TARGET).elf
	@echo ' --------------------------------------- '
	@echo '  '
  
%.rel: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDCC Compiler'
	$(CC) -c -mstm8  $(CFLAGS)  -o "$@" $(INCLUDE_DIRS) $< 
	@echo done compiling "$<"
	@echo '  '


flash: $(TARGET).ihx
	stm8flash -c stlinkv2 -p stm8s103?3  -w $(TARGET).ihx
	
clean: 
	@echo ' --------------------------------------- '
	@echo 'removing all $(OBJS)' 
	rm -rf $(TARGET).ihx $(OBJS)
	rm -rf $(TARGET).ls
	rm -rf $(TARGET).map
	rm -rf $(TARGET).rel
	rm -rf $(TARGRT).sym
	rm -rf $(TARGET).rst
	rm -rf $(TARGET).cdb
	rm -rf $(TARGET).elf
	rm -rf $(TARGET).lk
	rm -rf $(MAIN_C).rst
	rm -rf $(MAIN_C).sym
	rm -rf $(MAIN_C).lst
	rm -rf $(MAIN_C).asm

	rm -rf $(OBJS:.rel=.rst)
	rm -rf $(OBJS:.rel=.sym)
	rm -rf $(OBJS:.rel=.lst)
	rm -rf $(OBJS:.rel=.asm)
	rm -rf $(OBJS:.rel=.adb)

	