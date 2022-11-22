NAME = minishell
CC=gcc
CFLAGS= -Wall -Wextra -Werror -g3

VECTOR_SRCS = vector.c
VECTOR_INCLUDE = vector/

LEXER_SRCS = lexer.c lexer_helper.c lexer_double_quote.c lexer_dollar.c lexer_single_quote.c
LEXER_INCLUDE = lexer/

AST_SRCS = ast.c ast_heredoc.c ast_init_free.c
AST_INCLUDE = ast/

COMMAND_SRCS = command.c ft_command.c command_token.c command_close_fd.c command_open_fd.c command_types.c command_wait.c
COMMAND_INCLUDE = command/

PATH_FINDER_SRCS = path_finder.c
PATH_FINDER_INCLUDE = path_finder/

ENV_SRCS = environement_variable.c env_null.c env_free.c env_key.c env_last_status.c env_set_random.c env_set_shl_shlvl.c env_vars.c env_null_set_underscore.c
ENV_INCLUDE = environement_variable/

BUILTIN_SRCS = built_in.c export.c unset.c env.c pwd.c ft_echo.c ft_exit.c ft_cd.c
BUILTIN_INCLUDE = built_in/

SIGNAL_SRCS = signaling.c signaling_child.c
SIGNAL_INCLUDE = signaling/

HEREDOC_SRCS = heredoc.c heredoc_utils.c
HEREDOC_INCLUDE = heredoc/

CURRENT_SRCS = main.c
LIBFT_INCLUDE = libft/

SRCS = $(addprefix vector/, $(VECTOR_SRCS)) $(CURRENT_SRCS) \
		$(addprefix lexer/, $(LEXER_SRCS)) $(addprefix ast/, $(AST_SRCS)) \
		$(addprefix command/, $(COMMAND_SRCS)) $(addprefix path_finder/, $(PATH_FINDER_SRCS)) \
		$(addprefix $(ENV_INCLUDE), $(ENV_SRCS)) $(addprefix $(BUILTIN_INCLUDE), $(BUILTIN_SRCS)) \
		$(addprefix $(SIGNAL_INCLUDE), $(SIGNAL_SRCS)) $(addprefix $(HEREDOC_INCLUDE), $(HEREDOC_SRCS))

OBJ = $(SRCS:.c=.o)

all: $(NAME)

INCLUDE = -I $(ENV_INCLUDE) -I $(PATH_FINDER_INCLUDE) -I $(VECTOR_INCLUDE) -I $(LEXER_INCLUDE) -I $(AST_INCLUDE) -I $(COMMAND_INCLUDE) -I $(BUILTIN_INCLUDE) -I $(SIGNAL_INCLUDE) -I $(HEREDOC_INCLUDE) -I $(LIBFT_INCLUDE)
LDFLAGS = -L/usr/lib -lreadline -lbsd

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(NAME) : lib $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(NAME) $(LDFLAGS) ./libft/libft.a

lib:
	@make -C libft

clean:
	@make clean -C ./libft/
	-rm -f $(OBJ)

fclean: clean
	@make fclean -C ./libft/
	-rm -f $(NAME)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re lib