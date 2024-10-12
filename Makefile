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
ACTIVITY_5_SRCS = $(wildcard activity-5/*.c)
TP3_SRCS = $(wildcard TP3/*.c)

# Crea una lista de ejecutables correspondientes a los archivos .c
PROCESS_EXECS = $(patsubst 1-processes/%.c,$(BIN_DIR)/1-processes-%.out,$(PROCESS_SRCS))
THREAD_EXECS = $(patsubst 2-threads/%.c,$(BIN_DIR)/2-threads-%.out,$(THREAD_SRCS))
ACTIVITY_5_EXECS = $(patsubst activity-5/%.c,$(BIN_DIR)/activity-5-%.out,$(ACTIVITY_5_SRCS))
TP3_EXECS = $(patsubst TP3/%.c,$(BIN_DIR)/TP3-%.out,$(TP3_SRCS))

# Target 'all' para compilar todos los ejecutables
all: $(BIN_DIR) processes threads activity-5 TP3

# Crea la carpeta 'bin/' si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regla para compilar los ejecutables en la carpeta de 'processes'
processes: $(PROCESS_EXECS)
	@echo "Compilando ejecutables de procesos"

# Regla para compilar los ejecutables en la carpeta de 'threads'
threads: $(THREAD_EXECS)
	@echo "Compilando ejecutables de hilos"

# Regla para compilar los ejecutables en la carpeta de 'activity-5'
activity-5: $(ACTIVITY_5_EXECS)
	@echo "Compilando ejecutables de la actividad 5"

# Regla para compilar los ejecutables en la carpeta de 'TP3'
TP3: $(TP3_EXECS)
	@echo "Compilando ejecutables del TP3"

# Regla genérica para compilar cualquier archivo .c en un ejecutable .out en 'bin/'
$(BIN_DIR)/1-processes-%.out: 1-processes/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/2-threads-%.out: 2-threads/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/activity-5-%.out: activity-5/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/TP3-%.out: TP3/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Limpiar los archivos compilados
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.out" -type f -delete
	rm -rf $(BIN_DIR)
