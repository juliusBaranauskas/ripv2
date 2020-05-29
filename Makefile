CFLAGS = -O
CC = g++
SRC = main.cpp utils.cpp packet.cpp router.cpp
OBJ = $(SRC:.cpp = .o)

compilation: $(OBJ)
	$(CC) $(CFLAGS) -g -o compilation $(OBJ)
	
clean:
	rm -f core *.o 
