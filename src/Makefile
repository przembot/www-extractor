CC=g++
CFLAGS = -std=c++14 -Wall -O2 -lstdc++ -lboost_unit_test_framework
DEPS = querylexer.h htmllexer.h source.h queryparser.h
OBJ = source.o main.o querylexer.o queryparser.o htmlparser.o htmllexer.o
OBJTEST = source.o test.o querylexer.o queryparser.o htmlparser.o htmllexer.o
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
