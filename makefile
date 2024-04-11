CC = gcc
CCFLGS = -O3 -lclr	
OBJFILE = clusterfudge.o colourizer.o optionhandler.o main.o
SRCDIR = src

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJ_FILES = $(patsubst $(SRCDIR)/%.c, %.o, $(SOURCES))
TARGET = clr

.PHONY: all clean

all: $(TARGET)

debug: $(OBJ_FILES)
	$(CC) $(CCFLGS) -o $@ $^ -DDEBUG

$(TARGET): $(OBJ_FILES)
	$(CC) $(CCFLGS) -o $@ $^

%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $<

install: $(TARGET)
	sudo mv $(TARGET) /usr/bin

clean:
	rm -rf $(OBJ_FILES) $(TARGET)