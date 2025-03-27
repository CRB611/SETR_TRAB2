BUILD_DIR=build
DEFAULT_TARGET=$(BUILD_DIR)/main
OBJECTS = $(BUILD_DIR)/cmdproc.o $(BUILD_DIR)/main.o

CFLAGS =-Wall -Wfatal-errors -ggdb -pthread
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
build/unity.o: unity/unity.c unity/unity.h unity/unity_internals.h
	$(CC) $(CFLAGS) -c unity/unity.c -o build/unity.o 

#gerar test.o
build/test.o: test/test.c test/test.h
	$(CC) $(CFLAGS) -c test/test.c -o build/test.o 

clean:
	rm -f *.o $(DEFAULT_TARGET)

run:	$(DEFAULT_TARGET)
	./$(DEFAULT_TARGET)