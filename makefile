# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
LIB_NAME = libft.a
LIB = $(LIB_DIR)/$(LIB_NAME)

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Targets
NAME = program

# Rules
all: $(LIB) $(NAME)

$(LIB):
	@$(MAKE) -C $(LIB_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(MAKE) -C $(LIB_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIB_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re