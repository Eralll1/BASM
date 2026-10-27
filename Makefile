
CC = gcc
CFLAGS = -Wall -Wextra -I/usr/local/include -g -fsanitize=address,undefined 
LIBS = -lm -lraylib -lGL -lpthread -ldl -lrt -lX11



better: objects/better.o \
		objects/mylib.o \
		objects/read_file.o \
		objects/stack_canary.o \
		objects/functions.o \
		objects/operations.o \
		objects/op_helper.o \
		objects/helper.o \
		objects/asm.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

asm_main: objects/mylib.o \
		  objects/read_file.o \
		  objects/stack_canary.o \
		  objects/functions.o \
		  objects/operations.o \
		  objects/op_helper.o \
		  objects/helper.o \
		  objects/asm.o \
		  objects/asm_main.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)


objects/mylib.o: outer/mylib.c outer/mylib.h
	$(CC) $(CFLAGS) -c $< -o $@

objects/read_file.o: outer/read_file.c outer/read_file.h
	$(CC) $(CFLAGS) -c $< -o $@

objects/stack_canary.o: outer/stack_canary.c outer/stack_canary.h
	$(CC) $(CFLAGS) -c $< -o $@


objects/functions.o: globals/functions.c \
					 globals/functions.h \
					 globals/globals.h \
					 globals/processor.h \
					 globals/ray_glob.h
	$(CC) $(CFLAGS) -c $< -o $@


objects/operations.o: globals/operations.c
	$(CC) $(CFLAGS) -c $< -o $@

objects/op_helper.o: globals/op_helper.c \
					 globals/op_helper.h \
					 globals/globals.h \
					 globals/processor.h \
					 globals/ray_glob.h
	$(CC) $(CFLAGS) -c $< -o $@


objects/helper.o: compiler/helper.c \
				  compiler/helper.h \
				  globals/globals.h
	$(CC) $(CFLAGS) -c $< -o $@

objects/asm.o: compiler/asm.c \
			   compiler/asm.h \
			   globals/globals.h
	$(CC) $(CFLAGS) -c $< -o $@

objects/asm_main.o: compiler/asm_main.c
	$(CC) $(CFLAGS) -c $< -o $@

objects/better.o: stackPU/better.c \
				  globals/processor.h \
				  globals/ray_glob.h
	$(CC) $(CFLAGS) -c $< -o $@

globals/operations.c: globals/INSTSET \
					  globals/gen_op_codes.py
	python3 globals/gen_op_codes.py

.PHONY: clean all

clean:
	rm -rf objects/*.o && \
	rm *.out && \
	rm better asm)main

all: better asm_main