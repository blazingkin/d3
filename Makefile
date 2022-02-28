CC=gcc
AR=ar
CFLAGS=-g
OBJS=bin/demand_pager.o


all: bin/d3.a

bin/d3.a: $(OBJS)
	ar rcs $@ $^

bin/%.o: src/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -r bin