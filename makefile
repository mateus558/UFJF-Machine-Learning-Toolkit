memtest:
	g++ -O1 -g -std=c++14 includes/* src/* -o main
all:
	g++ -std=c++14 includes/* src/* -o main
release:
	g++ -Ofast -std=c++14 src/* includes/* -o UFJF-MLTK-Ofast-RELEASE
debug:
	g++ -g -std=c++14 src/Utils.cpp src/Data.cpp includes/Point.hpp src/main.cpp -o main
test:
	g++ -g -std=c++14 includes/* src/* teste.cpp -o teste
clean:
	rm -f main*
