# ---------------------------------------------------------------------------
# Project name

NAME	= test_dac

# Settings for compilator and linker

CC      = sdcc
CFLAGS  = -I./INCLUDE -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 

# Settings for system of autoincrement of build version

PROJECT  = $(shell type PROJECT)
VERSION  = $(shell type VERSION)
BUILD    = $(shell type BUILD)
TYPE     = $(shell type TYPE)

PROJNAME = ${PROJECT}-${VERSION}-${BUILD}-${TYPE}
TARBALL  = ${PROJNAME}.tar

# Settings for M3P

M3P		 = m3p
COMPORT	 = //./com3
COMLOG	 = $(COMPORT)_log.txt
BAUD	 = 9600	

# Directories with source

SRC_DIR = src
# ---------------------------------------------------------------------------

all: test_dac

clean:
	rm $(NAME).hex
	rm $(NAME).bin
	rm $(NAME).map
	rm $(NAME).mem
	rm $(NAME).lnk
	rm pm3p_*.txt
	rm com?_log.txt
	rm $(TARBALL).gz
	rm $(SRC_DIR)\*.asm
	rm $(SRC_DIR)\*.rel
	rm $(SRC_DIR)\*.rst
	rm $(SRC_DIR)\*.sym
	rm $(SRC_DIR)\*.lst 

load:
	$(M3P) lfile load.m3p


dist:
	tar -cvf $(TARBALL) --exclude=*.tar .
	gzip $(TARBALL)

term:
	$(M3P) echo $(COMLOG) $(BAUD)  openchannel $(COMPORT) +echo 6 term -echo bye



LIST_SRC = \
$(SRC_DIR)/adc.c \
$(SRC_DIR)/dac.c \
$(SRC_DIR)/sio.c \
$(SRC_DIR)/test_dac.c \
$(SRC_DIR)/buzz.c \
$(SRC_DIR)/max.c \
$(SRC_DIR)/kb.c \
$(SRC_DIR)/lcd.c \
$(SRC_DIR)/vect.c


LIST_OBJ = $(LIST_SRC:.c=.rel)

test_dac : $(LIST_OBJ) makefile
	$(CC) $(LIST_OBJ) -o test_dac.hex $(LFLAGS)
	$(M3P) hb166 test_dac.hex test_dac.bin bye


$(LIST_OBJ) : %.rel : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@  

