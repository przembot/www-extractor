CC=g++
CFLAGS = -std=c++14 -O2 -Wall -lstdc++ -lboost_unit_test_framework
DEPS = querylexer.h htmllexer.h source.h queryparser.h
OBJ = source.o main.o queryparser.o
OBJTEST = source.o test.o queryparser.o
CFLAGS += -I./

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

tests: $(OBJTEST)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o all tests
