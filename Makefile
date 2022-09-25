NAME = minishell
CC=gcc
CFLAGS= -Wall -Wextra -Werror -g3

VECTOR_SRCS = vector.c
VECTOR_INCLUDE = vector/

LEXER_SRCS = lexer.c
LEXER_INCLUDE = lexer/

AST_SRCS = ast.c
AST_INCLUDE = ast/

COMMAND_SRCS = command.c
COMMAND_INCLUDE = command/

CURRENT_SRCS = main.c

SRCS = $(addprefix vector/, $(VECTOR_SRCS)) $(CURRENT_SRCS) \
		$(addprefix lexer/, $(LEXER_SRCS)) $(addprefix ast/, $(AST_SRCS)) \
		$(addprefix command/, $(COMMAND_SRCS))
OBJ = $(SRCS:.c=.o)

all: $(NAME)

INCLUDE = -I $(VECTOR_INCLUDE) -I $(LEXER_INCLUDE) -I $(AST_INCLUDE) -I $(COMMAND_INCLUDE)
LDFLAGS = -L/usr/lib -lreadline

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(NAME) $(LDFLAGS)

clean:
	-rm -f $(OBJ)

fclean: clean
	-rm -f $(NAME)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re