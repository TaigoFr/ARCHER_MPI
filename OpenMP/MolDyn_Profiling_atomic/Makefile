# Makefile for the molecular dynamics code

#
# C compiler and options  
#
CC=     cc -h omp  
LIB=	-lm

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
