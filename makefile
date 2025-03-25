BUILD_DIR=build
DEFAULT_TARGET=$(BUILD_DIR)/main
OBJECTS = $(BUILD_DIR)/cmdproc.o $(BUILD_DIR)/main.o

CFLAGS = -g -Wall -O3
LDLIBS=
CC=gcc

# Generate spplication
build/main: build/main.o build/cmdproc.o build/unity.o
	$(CC) $(CFLAGS) -o build/main build/main.o build/cmdproc.o

#gerar cmdproc.o
build/cmdproc.o: src/cmdproc.c src/cmdproc.h
	$(CC) $(CFLAGS) -c src/cmdproc.c -o build/cmdproc.o

#gerar main.o
build/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o 

#gerar unity.o
build/unity.o: src/unity.c src/unity.h src/unity_internals.h
	$(CC) $(CFLAGS) -c src/unity.c -o build/unity.o 


clean:
	rm -f *.o $(DEFAULT_TARGET)

run:	$(DEFAULT_TARGET)
	./$(DEFAULT_TARGET)