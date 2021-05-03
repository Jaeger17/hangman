CFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings -Wfloat-equal

hangman:

.PHONY: debug
debug: CFLAGS += -g
debug: hangman

.PHONY: clean
clean:
	$(RM) *.o hangman