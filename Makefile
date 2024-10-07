# Variables
CC = gcc
CFLAGS = -Wall -pthread

# Encuentra todos los archivos .c en las carpetas
PROCESS_SRCS = $(wildcard 1-processes/*.c)
THREAD_SRCS = $(wildcard 2-threads/*.c)

# Crea una lista de ejecutables correspondientes a los archivos .c
PROCESS_EXECS = $(PROCESS_SRCS:.c=.out)
THREAD_EXECS = $(THREAD_SRCS:.c=.out)

# Target 'all' para compilar todos los ejecutables
all: processes threads

# Regla para compilar los ejecutables en la carpeta de 'processes'
processes: $(PROCESS_EXECS)
	@echo "Compilando ejecutables de procesos"

# Regla para compilar los ejecutables en la carpeta de 'threads'
threads: $(THREAD_EXECS)
	@echo "Compilando ejecutables de hilos"

# Regla genérica para compilar cualquier archivo .c en un ejecutable .out
%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

# Limpiar los archivos compilados
#rm -f 1-processes/*.o 2-threads/*.o 1-processes/*.out 2-threads/*.out
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.out" -type f -delete
