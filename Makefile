# mazwa programu
TARGET = exe/maze

# pliki zrodlowe
SRCS = main.c file_loading.c chunks_handling.c bin_chunks_handling.c path_finding.c move_chunk.c file_saving.c 

CC = gcc

# Flagi kompilacji
#CFLAGS =

# opcjs linkowania
LDFLAGS =

# kompilacja
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# czyszczenie plikow tymczasowych i wynikowych
clean:
	rm -f $(TARGET)

# aby uniknac konfliktow z nazwami plikow
.PHONY: clean
