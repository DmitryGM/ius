# ---------------------------------------------------------------------------
# Имя проекта

NAME	= test_dac

# Настройки компилятора и линкера

CC      = sdcc
CFLAGS  = -I./INCLUDE -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80

# Настройки системы автоинкремента версии сборки

PROJECT  = $(shell cat PROJECT)
VERSION  = $(shell cat VERSION)
BUILD    = $(shell cat BUILD)
TYPE     = $(shell cat TYPE)

PROJNAME = ${PROJECT}-${VERSION}-${BUILD}-${TYPE}
TARBALL  = ${PROJNAME}.tar

# Настройки M3P

M3P		 = m3p
COMPORT	 = /dev/ttyS0
COMLOG	 = $(COMPORT)_log.txt
BAUD	 = 9600

# Каталоги с исходными текстами

SRC_DIR = SRC
# ---------------------------------------------------------------------------

all: test_dac

clean:
	-rm -f  $(NAME).hex \
			$(NAME).bin \
			$(NAME).map \
			$(NAME).mem \
			$(NAME).lnk \
			pm3p_*.txt \
			com?_log.txt \
			$(TARBALL).gz \
			$(SRC_DIR)/*.asm \
			$(SRC_DIR)/*.rel \
			$(SRC_DIR)/*.rst \
			$(SRC_DIR)/*.sym \
			$(SRC_DIR)/*.lst

load:
	$(M3P) lfile load.m3p


dist:
	tar cvf $(TARBALL) *
	gzip $(TARBALL)

term:
	$(M3P) echo $(COMLOG) $(BAUD)  openchannel $(COMPORT) +echo 6 term -echo bye


LIST_SRC = \
$(SRC_DIR)/adc.c \
$(SRC_DIR)/dac.c \
$(SRC_DIR)/sio.c \
$(SRC_DIR)/test_dac.c

LIST_OBJ = $(LIST_SRC:.c=.rel)

test_dac : $(LIST_OBJ) makefile
	$(CC) $(LIST_OBJ) -o test_dac.hex $(LFLAGS)
	$(M3P) hb166 test_dac.hex test_dac.bin bye


$(LIST_OBJ) : %.rel : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@
