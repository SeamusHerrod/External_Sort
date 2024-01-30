CXX = g++
CXXFLAGS = -std=c++11 

all: main test

main: MainTest.cpp
	$(CXX) $(CXXFLAGS) -o Lab2 MainTest.cpp
	chmod +x run_script.sh
	./run_script.sh

test: myTest.cpp
	$(CXX) $(CXXFLAGS) -o myTest myTest.cpp
	./myTest
testSafe: myTest.cpp
	$(CXX) $(CXXFLAGS) -o myTest myTest.cpp
	valgrind --leak-check=full ./myTest

clean:
	rm -f Lab2 MainTest myTest *.o test*.txt buffer.txt load_buffer.txt  poly_buffer.txt output.txt tmp*.txt out_*.txt result_*.txt time_log.txt test_result

cleanT:
	rm -f test*.txt buffer.txt load_buffer.txt poly_buffer.txt tmp*.txt output.txt