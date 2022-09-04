NAME = minishell
CC=gcc
CFLAGS= -Wall -Wextra -Werror -g3

VECTOR_SRCS = vector.c
VECTOR_INCLUDE = vector/

LEXER_SRCS = lexer.c
LEXER_INCLUDE = lexer/

CURRENT_SRCS = main.c

SRCS = $(addprefix vector/, $(VECTOR_SRCS)) $(CURRENT_SRCS) $(addprefix lexer/, $(LEXER_SRCS))
OBJ = $(SRCS:.c=.o)

all: $(NAME)

INCLUDE = -I $(VECTOR_INCLUDE) -I $(LEXER_INCLUDE)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $< 

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

clean:
	-rm -f $(OBJS)

fclean: clean
	-rm -f $(NAME)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re