# Target library
lib := libuthread.a
CFLAGS := -Wall -Wextra -Werror -MMD
objs := queue.o preempt.o context.o uthread.o
CC := gcc

all: $(lib)

# dep tracking must be under 'all' rule
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

## TODO: Phase 1
$(lib): $(objs)
	ar rcs $(lib) $(objs)

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c -o $@ queue.c

preempt.o: preempt.c private.h
	$(CC) $(CFLAGS) -c -o $@ preempt.c

context.o: context.c private.h
	$(CC) $(CFLAGS) -c -o $@ context.c preempt.o

uthread.o: uthread.c uthread.h context.c preempt.c private.h
	$(CC) $(CFLAGS) -c -o $@ uthread.c context.o

clean:
	rm -f $(objs) $(lib) $(deps)
