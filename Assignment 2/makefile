all: assign2.o

assign2.o: assign2.c
	gcc assign2.c -o assign2.o -g

plot: 
	python plot.py outfile

clean:
	rm -f *.o out*