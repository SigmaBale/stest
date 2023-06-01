BIN = build/release/libstest.so
INCDIRS = src include
SRCDIR = src
OBJDIR = build/obj

HEADER_INSTALL_DIR = /usr/local/include
LIB_INSTALL_DIR = /usr/local/lib

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
	@sudo cp -i include/stest.h $(HEADER_INSTALL_DIR)
	@sudo cp -i build/release/libstest.so $(LIB_INSTALL_DIR) 

uninstall:
	@sudo rm $(HEADER_INSTALL_DIR)/stest.h
	@sudo rm $(LIB_INSTALL_DIR)/libstest.so

-include $(DEPFILES)

.PHONY: all clean install uninstall
