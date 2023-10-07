#Sarah Martin C00257967
# Makefile for rendezvous.cpp and Semaphore.cpp

CC=g++
CFLAGS= -std=c++11 -pthread

Rendezvous: Semaphore.cpp rendezvous.cpp 
	$(CC) Semaphore.cpp rendezvous.cpp -Wall -o Rendezvous

CLEAN:
	rm -f Rendezvous

