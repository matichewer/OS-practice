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

# Buscar archivos .c recursivamente en todas las carpetas
SRCS = $(shell find . -type f -name "*.c")

# Crear lista de ejecutables, colocando cada uno en su subdirectorio dentro de "bin"
EXECS = $(patsubst ./%.c, $(BIN_DIR)/%.out, $(SRCS))

# Target 'all' para compilar todos los ejecutables
all: $(BIN_DIR) $(EXECS)

# Crear la carpeta 'bin/' si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regla genérica para compilar cada archivo .c en un ejecutable .out en 'bin'
$(BIN_DIR)/%.out: %.c | $(BIN_DIR)
	@mkdir -p $(dir $@)  # Crea subdirectorios dentro de 'bin' si es necesario
	$(CC) $(CFLAGS) "$<" -o "$@"

# Limpiar los archivos compilados
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.out" -type f -delete
	rm -rf $(BIN_DIR)

# Show help
help:
	@echo "Available options for 'make':"
	@echo "  make             - Compiles all source files."
	@echo "  make clean       - Removes binaries and intermediate files."
	@echo "  make help        - Displays this help message."
	@echo ""
	@echo "Optional variables:"
	@echo "  WITH_WALL=1      - Adds '-Wall' to the compiler flags for more warnings."

