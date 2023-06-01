BIN = build/release/libstest.so
INCDIRS = src include
SRCDIR = src
OBJDIR = build/obj

CC = gcc
INCLUDES := $(foreach dir,$(INCDIRS),-I$(dir))
DEPFLAGS = -MD -MP
OPT_BUILD = -Os
CFLAGS := -Wall -Werror -Wextra $(OPT_BUILD) $(INCLUDES) $(DEPFLAGS)

SRCFILES := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))
DEPFILES := $(patsubst %.o,%.d,$(OBJFILES))

all: $(BIN)

$(BIN): $(OBJFILES)
	@$(CC) -shared -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c -fPIC $< -o $@ 

clean:
	@rm -rf $(OBJFILES) $(DEPFILES) $(BIN)

install: $(BIN)
	@sudo cp -i include/stest.h /usr/local/include
	@sudo cp -i build/release/libstest.so /usr/local/bin

uninstall:
	@sudo rm /usr/local/include/stest.h
	@sudo rm /usr/local/bin/libstest.so

.PHONY: all clean install uninstall
