# Target library
lib := libuthread.a

all: $(lib)

## TODO: Phase 1
$(lib): queue.o uthread.o context.o preempt.o
	ar rcs $(lib) queue.o

queue.o: queue.c queue.h
	gcc -Wall -Werror -Wextra -c -o queue.o queue.c

preempt.o: preempt.c private.h
	gcc -Wall -Werror -Wextra -c -o preempt.o preempt.c

context.o: context.c private.h
	gcc -Wall -Werror -Wextra -c -o context.o context.c preempt.o

uthread.o: uthread.c uthread.h context.c preempt.c private.h
	gcc -Wall -Werror -Wextra -c -o uthread.o uthread.c context.o

clean:
	rm -f uthread.o queue.o libuthread.a
