CFLAGS = -Wall -g -ggdb -Iinclude -Llib
LFLAGS = -ltree-sitter -ljson-c
OBJS = $(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))
DEPS = $(wildcard include/*.h)

obj/%.o: src/%.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

main: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)