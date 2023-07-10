all: test generate

test: test.cpp matrix/Matrix.cpp matrix/Matrix.h matrix/MatrixManager.cpp matrix/MatrixManager.h
	g++ -std=c++14 -Wall -pedantic test.cpp matrix/Matrix.cpp matrix/MatrixManager.cpp -lpthread -o test

generate: generator.py
	python3 generator.py

clean:
	rm -rf temp/* tests/* test
