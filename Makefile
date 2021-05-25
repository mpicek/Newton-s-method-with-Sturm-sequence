main: main.cpp
	g++ --std=c++11 -o main main.cpp
test: main
	python3 integration_test.py ./main