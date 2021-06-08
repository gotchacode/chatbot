all:
	clang -std=c17 -Wall -Wextra -Wno-nullability-extension -Wno-nullability-completeness -pedantic src/chatbot.c -o chat

clean:
	rm -rf *o chat
