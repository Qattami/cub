CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

SRC_DIRS = parsing src

# Find all .c files in SRC_DIRS
SRC = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

OBJ = $(SRC:.c=.o)

NAME = my_program

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
