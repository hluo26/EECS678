STUDENT_ID=2737588

all: client server

%: %.c
	gcc -g $^ -o $@ -lm

test: client server
	bash -c "./server & sleep 1; ./client"

clean:
	rm -f client server mysock

tar: clean
	mkdir $(STUDENT_ID)-sockets-lab
	cp client.c server.c Makefile $(STUDENT_ID)-sockets-lab/
	tar cvzf $(STUDENT_ID)-sockets-lab.tar.gz $(STUDENT_ID)-sockets-lab
	rm -rf $(STUDENT_ID)-sockets-lab

.PHONY: all test clean tar
