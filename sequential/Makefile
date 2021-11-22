all: sequential_test convert

seq_floyd: ./src/seq_floyd.cpp ./src/seq_floyd_test.cpp
	g++ -o seq_floyd ./src/seq_floyd.cpp ./src/seq_floyd_test.cpp

sequential_test: ./src/sequential_test.cpp ./src/utils.cpp ./src/seq_mat_mul.cpp
	g++ -o sequential_test ./src/sequential_test.cpp ./src/utils.cpp ./src/seq_mat_mul.cpp

convert: ./src/convert.cpp ./src/utils.cpp ./src/seq_mat_mul.cpp
	g++ -o convert ./src/convert.cpp ./src/utils.cpp ./src/seq_mat_mul.cpp

clean:
	rm -f *.o sequential_test convert
