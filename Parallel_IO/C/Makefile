MF=	Makefile

#CC=	mpicc
CC=	cc
#CFLAGS=	-cc=icc

LFLAGS=

EXE=	mpiio

SRC= \
	mpiio.c \
	ioutils.c

INC= \
	ioutils.h

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .c .o

OBJ=	$(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE) cdat2pgm

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(INC) $(MF)

cdat2pgm: dat2pgm.c
	cc -D ORDER_C -o cdat2pgm dat2pgm.c

clean:
	rm -f $(OBJ) $(EXE) core cdat2pgm
