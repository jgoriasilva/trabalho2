# Macros para compilacao
CC = gcc
CFLAGS = -lfreeimage -lpthread
DIR = src
FILENAME = $(DIR)/main.c
TARGET = ./main
BJS = $(SRCS:.c=.o)

# Macros para teste
# BASH = sh
# TEST_SCRIPT = test.sh
# VERBOSE ?= 1

# Macros para construcao do zip
ZIP = zip
USERNAME ?= $(USER)
ZIPFILE = $(USERNAME).zip
EXTENSIONS = *.c *.h *.in *.out *.sh

.PHONY: depend clean

all:$(TARGET)

$(TARGET):$(TARGET)
	$(CC) -o$(TARGET) $(CFLAGS)

test:all
	$(BASH) $(TEST_SCRIPT) $(TARGET) $(VERBOSE)

zip:clean
	$(ZIP) -R $(ZIPFILE)  Makefile $(EXTENSIONS)

run:all
	$(TARGET)

clean:
	$(RM) $(YYTABC)
	$(RM) $(YYTABH)
	$(RM) $(LEXOUT)
	$(RM) ./$(TARGET)
	$(RM) $(DIR)/*.o
	$(RM) ./$(ZIPFILE)
