CC=clang
INC=./include/
OPTFLAGS = 

default: test

test: clean
	if [ ! -d bin ]; then mkdir bin; fi
	$(CC) $(OPTFLAGS) -I$(INC) test/*.c src/*.c -o bin/test
	./bin/test
clean:
	if [ -f bin/test ] ; then rm bin/test; fi