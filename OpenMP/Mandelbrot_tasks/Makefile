# Makefile for mandelbrot area code

#
# C compiler and options for Cray
#
CC=	cc
LIB=

#
# C compiler and options for Intel
#
#CC=     icc -O3 -openmp
#LIB=    -lm

#
# C compiler and options for PGI 
#
#CC=     pgcc -O3 -mp
#LIB=	-lm

#
# C compiler and options for GNU 
#
#CC=     gcc -O3 -fopenmp
#LIB=	-lm

#
# Object files
#
OBJ=    area.o

#
# Compile
#
area:   $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIB)

.c.o:
	$(CC) -c $<

#
# Clean out object files and the executable.
#
clean:
	rm *.o area
