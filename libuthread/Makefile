# Target library
lib 	:= libuthread.a
objs    := queue.o uthread.o sem.o
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g -MMD
ifneq ($(V),1)
Q = @
endif
all: $(lib)
libuthread.a: $(objs)
	ar rcs $@ $^
%.o: %.c
	$(Q)$(CC) $(CFLAGS) -c -o $@ $^
clean:
	rm -f $(lib) $(objs)
