# Target library
lib 	:= libuthread.a
objs    := queue.o uthread.o sem.o context.o preempt.o
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -MMD -g

all: $(lib)

# Dep tracking *must* be below the 'all' rule
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

libuthread.a: $(objs)
	ar rcs $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
clean:
	rm -f $(lib) $(objs) $(deps)
