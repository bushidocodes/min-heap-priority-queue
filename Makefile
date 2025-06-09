CC=clang
INC=./include/
OPTFLAGS = -g

ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

default: test

test: clean
	@if [ ! -d bin ]; then mkdir bin; fi
	@$(CC) $(OPTFLAGS) -I$(INC) test/*.c test/vendor/*.c src/*.c -o bin/test
	@./bin/test
clean:
	@if [ -f bin/test ] ; then rm bin/test; fi

.PHONY: memcheck
memcheck: test/*.c src/*.c
	@mkdir -p ./bin
	@$(CC) $(ASANFLAGS) $(OPTFLAGS) -I$(INC) test/*.c test/vendor/*.c src/*.c -o bin/memcheck.out $(LIBS)
	@./bin/memcheck.out
	@echo "Memory check passed"

format:
	@clang-format -style=file -i src/* include/*
