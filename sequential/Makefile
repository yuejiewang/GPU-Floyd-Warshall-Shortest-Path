sequential_test: ./include/utils.o ./src/main.cpp
	g++ -o sequential_test ./src/main.cpp ./include/utils.o

utils: ./include/utils.h ./src/utils.cpp
	g++ -o ./include/utils.o -c ./src/utils.cpp

clean:
	rm -f *.o sequential_test