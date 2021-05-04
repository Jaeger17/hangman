CFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings -Wfloat-equal

src/hangman:

.PHONY: debug
debug: CFLAGS += -g
debug: src/hangman

.PHONY: clean
clean:
	$(RM) src/*.o src/hangman .hangman