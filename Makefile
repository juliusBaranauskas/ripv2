CFLAGS = -O
CC = g++
SRC = main.cpp utils.cpp packet.cpp router.cpp
OBJ = $(SRC:.cpp = .o)

compilation: $(OBJ)
	$(CC) $(CFLAGS) -o compilation $(OBJ)
	
clean:
	rm -f core *.o 
