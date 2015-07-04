target = final_project
CC = g++
CPPFLAGS = -c -g -Wall -O2 -std=c++11

all: clean $(target)


$(target): main.o md5.o function.o auxiliary.o
	$(CC) md5.o main.o function.o auxiliary.o -o final_project

main.o: main.cc main.h function.h auxiliary.h 
	$(CC) $(CPPFLAGS) main.cc


md5.o: md5.cc md5.h
	$(CC) $(CPPFLAGS) md5.cc

function.o: main.h function.cc function.h auxiliary.h md5.h
	$(CC) $(CPPFLAGS) function.cc

auxiliary.o: main.h auxiliary.cc auxiliary.h
	$(CC) $(CPPFLAGS) auxiliary.cc	


clean:
	rm -f *.o
	rm -f $(target)
