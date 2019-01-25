# ---------------------------------------------------------------------------
# Имя проекта

NAME	= test_dac

# Настройки компилятора и линкера

CC      = sdcc
CFLAGS  = -I./include -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80

# Настройки системы автоинкремента версии сборки

PROJECT  = $(shell type PROJECT)
VERSION  = $(shell type VERSION)
BUILD    = $(shell type BUILD)
TYPE     = $(shell type TYPE)

PROJNAME = ${PROJECT}-${VERSION}-${BUILD}-${TYPE}
TARBALL  = ${PROJNAME}.tar

# Настройки M3P

M3P		 = m3p
COMPORT	 = com1
COMLOG	 = $(COMPORT)_log.txt
BAUD	 = 9600

# Каталоги с исходными текстами

SRC_DIR = src
# ---------------------------------------------------------------------------

all: test_dac

clean:
	del $(SRC_DIR)\*.asm
	del $(SRC_DIR)\*.rel
	del $(SRC_DIR)\*.rst
	del $(SRC_DIR)\*.sym
	del $(SRC_DIR)\*.lst 
	del $(NAME).hex
	del $(NAME).bin
	del $(NAME).map
	del $(NAME).mem
	del $(NAME).lnk
	del pm3p_*.txt
	del com?_log.txt
	# del $(TARBALL).gz


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
