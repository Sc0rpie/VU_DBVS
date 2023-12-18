CC := gcc
CFLAGS := -Wall -I/opt/homebrew/Cellar/libpq/16.1_1/include
LIBS := -L/opt/homebrew/Cellar/libpq/16.1_1/lib -lpq
TARGET := app

# List your .c files here
SRC := main.c menu.c sql.c messages.c

# Object files corresponding to .c files
OBJ := $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Compile and link the program
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Compile each .c file into an .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJ)
