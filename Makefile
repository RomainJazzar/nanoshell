# Name of the executable
NAME = nanoshell

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
SRC = nanoshell.c

# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(NAME)

# Link the program
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files
clean:
	rm -f $(OBJ)

# Clean up everything (objects + executable)
fclean: clean
	rm -f $(NAME)

# Re-compile everything
re: fclean all

.PHONY: all clean fclean re