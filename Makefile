###########################################################
# Makefile for CS570 Assignment #3
# Cody Morgan, Andrew Levandowski
###########################################################

CC = g++
CPPFILES = driver.cpp Timer.cpp
FLAG = -std=c++11
EXEC = tevent

build:
	$(CC) $(CPPFILES) -o $(EXEC) $(FLAG)
	rm -f *.o

clean:
	rm -f *.o core a.out $(EXEC)

#######################[ EOF: Makefile ]###################
