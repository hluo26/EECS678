# Defined constants for command base name and arguments for simple test
#
STUDENT_ID=2737588
LOOP=100000000
LOOP_HELGRIND=1
INC=1

all: ptcount_mutex ptcount_atomic

ptcount_mutex: ptcount_mutex.c
	gcc -g -o $@ $^ -lpthread

ptcount_atomic: ptcount_atomic.c
	gcc -g -o $@ $^ -lpthread

test: all
	time ./ptcount_mutex $(LOOP) $(INC)
	time ./ptcount_atomic $(LOOP) $(INC)

test-helgrind: all
	valgrind --tool=helgrind ./ptcount_mutex $(LOOP_HELGRIND) $(INC)
	valgrind --tool=helgrind ./ptcount_atomic $(LOOP_HELGRIND) $(INC)

clean:
	rm -f ptcount_mutex ptcount_atomic

tar:
	make clean
	mkdir $(STUDENT_ID)-pthreads_intro-lab
	cp ptcount_mutex.c ptcount_atomic.c Makefile $(STUDENT_ID)-pthreads_intro-lab/
	tar cvzf $(STUDENT_ID)-pthreads_intro-lab.tar.gz $(STUDENT_ID)-pthreads_intro-lab
	rm -rf $(STUDENT_ID)-pthreads_intro-lab

