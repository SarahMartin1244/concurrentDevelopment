# Makefile for Barrier.cpp and Semaphore.cpp
# Author: Sarah Martin (C00257967)
# License: This project is licensed under the [GNU General Public License, Version 3](https://www.gnu.org/licenses/gpl-3.0).
# Git URL: https://github.com/SarahMartin1244/concurrentDevelopment/tree/main/labThree

CC=g++
# Compiler flags
CFLAGS=-std=c++11 -pthread -I.
# Debug flags
DEBUGFLAGS=
# Target: %.o depends on %.cpp
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

# Target: ALL Semaphore.o, Barrier.o, and main.o
ALL: Semaphore.o Barrier.o main.o
	$(CC) -o barrier Semaphore.o Barrier.o main.o $(CFLAGS) $(DEBUGFLAGS)

# Target: sets DEBUGFLAGS 
DEBUG: DEBUGFLAGS = -g -O0
DEBUG: ALL

# Target: CLEAN removes object files and the exe
CLEAN:
	rm -f *.o barrier

