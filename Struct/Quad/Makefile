
CC=gcc
CFLAGS=-I. -std=c99 -Wall -lquadmath -lpthread -Ofast -O3 -m64 -march=native -fipa-pure-const -fipa-reference -fmerge-constants -fshrink-wrap -fsplit-wide-types -ftree-builtin-call-dce -ftree-copyrename -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-sra -ftree-pta -ftree-ter -funit-at-a-time -fkeep-inline-functions -fkeep-static-consts -fmerge-constants -fmodulo-sched -fgcse -fdce -fdse -fexpensive-optimizations -fipa-cp-clone -fipa-matrix-reorg -ftree-loop-linear -floop-interchange -floop-strip-mine -floop-block -ftree-loop-distribution -ffast-math -fassociative-math -freciprocal-math -funroll-all-loops -fpeel-loops 
DEPS =graf_ER.h double_random.h
OBJ =graf_ER.o double_random.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

percolation: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
