main:main.o judge.o input.o output.o 
	g++ -o main main.o judge.o input.o output.o -g
main.o:main.cpp
	g++ -c -o main.o main.cpp
input.o:input.cpp
	g++ -c -o input.o input.cpp
output.o:output.cpp
	g++ -c -o output.o output.cpp
judge.o:judge.cpp
	g++ -c -o judge.o judge.cpp
clean:
	-rm *.o
	-rm main
