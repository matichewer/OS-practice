# Variables
CC = gcc
CFLAGS = -Wall -pthread
BIN_DIR = bin

# Encuentra todos los archivos .c en las carpetas
PROCESS_SRCS = $(wildcard 1-processes/*.c)
THREAD_SRCS = $(wildcard 2-threads/*.c)

# Crea una lista de ejecutables correspondientes a los archivos .c
PROCESS_EXECS = $(patsubst 1-processes/%.c,$(BIN_DIR)/1-processes-%.out,$(PROCESS_SRCS))
THREAD_EXECS = $(patsubst 2-threads/%.c,$(BIN_DIR)/2-threads-%.out,$(THREAD_SRCS))

# Target 'all' para compilar todos los ejecutables
all: $(BIN_DIR) processes threads

# Crea la carpeta 'bin/' si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regla para compilar los ejecutables en la carpeta de 'processes'
processes: $(PROCESS_EXECS)
	@echo "Compilando ejecutables de procesos"

# Regla para compilar los ejecutables en la carpeta de 'threads'
threads: $(THREAD_EXECS)
	@echo "Compilando ejecutables de hilos"

# Regla genérica para compilar cualquier archivo .c en un ejecutable .out en 'bin/'
$(BIN_DIR)/1-processes-%.out: 1-processes/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/2-threads-%.out: 2-threads/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Limpiar los archivos compilados
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.out" -type f -delete
	rm -rf $(BIN_DIR)
