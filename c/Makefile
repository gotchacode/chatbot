CC ?= cc
CFLAGS = -std=c23 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wstrict-prototypes -O2
SRC = src/chatbot.c

.PHONY: all chat clean info

all: chat

chat: $(SRC) src/chatbot.h
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	rm -f chat

info:
	@echo "Compiler: $(CC)"
	@echo "Flags:    $(CFLAGS)"
