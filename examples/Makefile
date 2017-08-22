ifeq ($(shell uname -s),Darwin)
CXX := clang++
else
CXX := g++
endif

INCLUDE := -I../

all: 
	$(CXX) $(INCLUDE) --std=c++14 test.cpp -o test
	$(CXX) $(INCLUDE) --std=c++14 mt-example.cpp -o example

clean:
	@rm -f *.o
