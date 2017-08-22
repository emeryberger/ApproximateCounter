ifeq ($(shell uname -s),Darwin)
CXX := clang++
else
CXX := g++
endif

INCLUDE := -I../

all: 
	$(CXX) $(INCLUDE) --std=c++14 test.cpp -o test.o

clean:
	@rm -f *.o
