KEYSTONE_LDFLAGS := -lkeystone -lstdc++ -lm
SDL_FLAGS := -L/usr/local/lib -lSDL2main -lSDL2 
CFLAGS	:=  -std=c++17 -O3 -ggdb -DDEBUG $(KEYSTONE_LDFLAGS)

CC = g++ 
SRC := ../src
OBJS := objmemory.o bitblt.o main.o interpreter.o jit.o

Smalltalk: $(OBJS) 
	$(CC) -o $@ $^ $(SDL_FLAGS)	

main.o: $(SRC)/main.cpp
	$(CC) $(CFLAGS) -c $(SRC)/main.cpp 

%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $<  -o $@


clean:
	rm -f *.o Smalltalk

run:
	./Smalltalk -directory ../files -three

dbg:
	gdb --args ./Smalltalk -directory ../files -three

