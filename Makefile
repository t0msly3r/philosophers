CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = src/main.c src/utils.c src/utils_2.c src/threads.c
OBJ = $(SRC:.c=.o)
NAME = philo

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re