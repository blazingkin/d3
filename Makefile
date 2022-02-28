CC=gcc
AR=ar
CFLAGS=-g
OBJS=bin/demand_pager.o


all: bin/demand_pager.a

bin/demand_pager.a: $(OBJS)
	ar rcs $@ $^

bin/%.o: src/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -r bin