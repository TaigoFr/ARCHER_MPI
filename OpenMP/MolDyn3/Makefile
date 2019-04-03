# Makefile for the molecular dynamics code

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
OBJ=    main.o \
	dfill.o \
	domove.o \
        dscal.o \
	fcc.o \
	forces.o \
	mkekin.o \
	mxwell.o \
	prnout.o \
	velavg.o

#
# Compile
#
md:	$(OBJ)
	$(CC) -o $@ $(OBJ) $(LIB)

.c.o:
	$(CC) -c $<

#
# Clean out object files and the executable.
#
clean:
	rm *.o md
