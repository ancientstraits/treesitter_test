CFLAGS = -Wall -g -ggdb -Iinclude -Llib
LFLAGS = -ltree-sitter -ljson-c
OBJS = $(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))
DEPS = $(wildcard include/*.h)

obj/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

main: $(OBJS) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)