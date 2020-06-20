##
## $URL: https://repo1.mydevil.net/svn/priv/bawolski/stm8/trunk/tig_controller/Makefile $      
## $Revision: 31 $ 

CC=sdcc
ROOT_PROJ=$(strip $(CURDIR))
INCLUDE_DIRS=-I$(ROOT_PROJ) -I$(ROOT_PROJ)/misc/ -I$(ROOT_PROJ)/mode2T/

TARGET:=led
MAIN_C=main

OUT_FOLDER:=build
OBJS=
C_SRCS=

C_SRCS+=$(MAIN_C).c
OBJS+=$(MAIN_C).rel

CFLAGS= -DSTM8S103  --all-callee-saves --debug --verbose --stack-auto --fverbose-asm  --float-reent --no-peep

-include ./misc/misc.mk

.PHONY: all flash

all: $(TARGET).ihx Makefile

$(TARGET).ihx:  $(OBJS)
# $(C_SRCS) 
	@echo ' --------------------------------------- '
	@echo 'Final compile and link $@'
	$(CC) -o $(TARGET).ihx $(CFLAGS) --verbose -V  -mstm8 $(INCLUDE_DIRS) --out-fmt-ihx $(OBJS)
	$(CC) -o $(TARGET).elf $(CFLAGS) --verbose -V  -mstm8 $(INCLUDE_DIRS) --out-fmt-elf $(OBJS)
	stm8-size  $(TARGET).elf
#	$(CC)  --verbose -V  -mstm8 $(INCLUDE_DIRS)  $(OBJS) 
# $(MAIN_C)
	@echo ' --------------------------------------- '
	@echo '  '


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

  
%.rel: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDCC Compiler'
	$(CC) -c -mstm8  $(CFLAGS) --verbose -V -o "$@" $(INCLUDE_DIRS) $< 
	@echo done compiling "$<"
	@echo '  '


flash: $(TARGET).ihx

	#stm8flash -c stlinkv2 -p stm8s103?3  -w $(TARGET).ihx
	#/cygdrive/c/Program\ Files\ \(x86\)/STMicroelectronics/st_toolset/stvp/STVP_CmdLine.exe --help
	'C:\Program Files (x86)\STMicroelectronics\st_toolset\stvp\STVP_CmdLine.exe' -BoardName=ST-LINK  -Device=STM8S103F3 -no_loop -FileProg=$<
	
	
	