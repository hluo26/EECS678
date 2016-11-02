STUDENT_ID=2737588

all:
	gcc -g signals.c -o signals

test:
	./signals

clean:
	rm -f *.o signals

tar: clean
	make clean
	mkdir $(STUDENT_ID)-signals-lab
	cp signals.c Makefile $(STUDENT_ID)-signals-lab/
	tar cvzf $(STUDENT_ID)-signals-lab.tar.gz $(STUDENT_ID)-signals-lab
	rm -rf $(STUDENT_ID)-signals-lab

