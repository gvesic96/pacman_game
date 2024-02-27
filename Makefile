# Compiler and compiler flags
CC = mpic++
CFLAGS = -Wall

# Source file and executable name
SOURCE = pacman_mpi.cpp
EXECUTABLE = pacman_mpi

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	$(CC) -o $(EXECUTABLE) $(SOURCE)

clean:
	rm -f $(EXECUTABLE)
#mpiexec -n <num_threads> ./pacman_mpi
