sequential_test: ./src/main.cpp
	g++ -o sequential_test ./src/main.cpp ./src/utils.cpp ./src/seq_mat_mul.cpp

# utils: ./include/utils.h ./src/utils.cpp ./include/seq_mat_mul.h ./src/seq_mat_mul.cpp
# 	g++ -o ./include/utils.o -c ./src/utils.cpp
#	g++ -o ./include/seq_mat_mul.o -c ./src/seq_mat_mul.cpp

clean:
	rm -f *.o sequential_test
