target = final_project
CC = g++
CPPFLAGS = -c -g -Wall -O2 -std=c++11

all: clean $(target)

$(target): account.o function.o auxiliary.o trie.o md5.o main.o
	$(CC) account.o function.o auxiliary.o trie.o md5.o main.o -o final_project

main.o: main.cc md5.h trie.h auxiliary.h function.h account.h
	$(CC) $(CPPFLAGS) main.cc

md5.o: md5.cc md5.h
	$(CC) $(CPPFLAGS) md5.cc

function.o: function.cc function.h auxiliary.h account.h
	$(CC) $(CPPFLAGS) function.cc

trie.o: trie.cc trie.h account.h
	$(CC) $(CPPFLAGS) trie.cc

auxiliary.o: auxiliary.cc auxiliary.h account.h trie.h
	$(CC) $(CPPFLAGS) auxiliary.cc	

account.o: account.cc account.h
	$(CC) $(CPPFLAGS) account.cc

clean:
	rm -f *.o
	rm -f $(target)
