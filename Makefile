GTEST_DIR=/home/ricbit/src/gtest-1.6.0
all: matrix_test modint_test bigint_test io_test primes_test fibonacci_test chinese_test bintree_test
%_test : %_test.cc %.h
	g++ -std=c++14 -I${GTEST_DIR}/include -L${GTEST_DIR}/make $< -o $@ ${GTEST_DIR}/make/gtest_main.a -lpthread -Wall -g
