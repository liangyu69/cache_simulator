CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -Wno-unused-result
TARGET = cache_sim
SRCS = src/main.c src/cache_core.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run