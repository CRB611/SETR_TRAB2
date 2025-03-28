CFLAGS =-Wall -Wfatal-errors -ggdb -pthread
CC=gcc

TARGET = main

.PHONY: all clean cleanall

all: $(TARGET)

main: build/main.o build/cmdproc.o build/test.o build/unity.o
	$(CXX) $(CFLAGS) -o $@ $^ 

teste: build/teste.o build/cmdproc.o build/test.o build/unity.o
	$(CXX) $(CFLAGS) -o $@ $^ 

build/teste.o: src/teste.c
	$(CXX) $(CFLAGS) -c $< -o $@

build/main.o: src/main.c
	$(CXX) $(CFLAGS) -c $< -o $@

build/cmdproc.o: src/cmdproc.c src/cmdproc.h
	$(CXX) $(CFLAGS) -c $< -o $@

build/test.o: tests/test.c tests/test.h 
	$(CXX) $(CFLAGS) -c $< -o $@

build/unity.o: unity/unity.c unity/unity.h unity/unity_internals.h 
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(DEFAULT_TARGET)

run:	$(DEFAULT_TARGET)
	./$(DEFAULT_TARGET)
