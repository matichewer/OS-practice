# Compilar con y sin -Wall
# make
# make WITH_WALL=1


# Variables
CC = gcc
CFLAGS_BASE = -pthread
CFLAGS_WALL = -Wall
BIN_DIR = bin

# Variable condicional para activar o desactivar -Wall
ifeq ($(WITH_WALL),1)
	CFLAGS = $(CFLAGS_WALL) $(CFLAGS_BASE)
else
	CFLAGS = $(CFLAGS_BASE)
endif

# Encuentra todos los archivos .c en las carpetas
PROCESS_SRCS = $(wildcard 1-processes/*.c)
THREAD_SRCS = $(wildcard 2-threads/*.c)
ACTIVIDAD_5_SRCS = $(wildcard actividad-5/*.c)

# Crea una lista de ejecutables correspondientes a los archivos .c
PROCESS_EXECS = $(patsubst 1-processes/%.c,$(BIN_DIR)/1-processes-%.out,$(PROCESS_SRCS))
THREAD_EXECS = $(patsubst 2-threads/%.c,$(BIN_DIR)/2-threads-%.out,$(THREAD_SRCS))
ACTIVIDAD_5_EXECS = $(patsubst actividad-5/%.c,$(BIN_DIR)/actividad-5-%.out,$(ACTIVIDAD_5_SRCS))

# Target 'all' para compilar todos los ejecutables
all: $(BIN_DIR) processes threads actividad-5

# Crea la carpeta 'bin/' si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regla para compilar los ejecutables en la carpeta de 'processes'
processes: $(PROCESS_EXECS)
	@echo "Compilando ejecutables de procesos"

# Regla para compilar los ejecutables en la carpeta de 'threads'
threads: $(THREAD_EXECS)
	@echo "Compilando ejecutables de hilos"

# Regla para compilar los ejecutables en la carpeta de 'actividad-5'
actividad-5: $(ACTIVIDAD_5_EXECS)
	@echo "Compilando ejecutables de la actividad 5"

# Regla genérica para compilar cualquier archivo .c en un ejecutable .out en 'bin/'
$(BIN_DIR)/1-processes-%.out: 1-processes/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/2-threads-%.out: 2-threads/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/actividad-5-%.out: actividad-5/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Limpiar los archivos compilados
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.out" -type f -delete
	rm -rf $(BIN_DIR)
