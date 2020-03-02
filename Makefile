CC=g++

all: sqrbank genlist

demo: all
	./genlist 10000000 100 | pv -s 321807080 | ./sqrbank -

sqrbank: sqrbank.cc
	$(CC) -Wall -Wextra -std=c++11 -O3 -flto -o sqrbank sqrbank.cc

genlist: genlist.cc
	$(CC) -Wall -Wextra -std=c++11 -O3 -flto -o genlist genlist.cc

clean:
	rm -rf sqrbank
	rm -rf genlist
