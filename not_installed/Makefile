# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mportrai <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/16 12:30:44 by mportrai          #+#    #+#              #
#    Updated: 2022/06/13 18:15:45 by mportrai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRC_PATH	= ./src/
OBJ_PATH	= ./obj/
BNS_PATH	= ./bonus/
OBJB_PATH	= ./objbonus/
H_PATH		= -I./includes/

SRC_NAME	= ft_pipex.c ft_init_value.c ft_path.c ft_possible_path.c ft_error_pipefork.c ft_error_cmd.c
OBJ_NAME	= ${SRC_NAME:.c=.o}
BNS_NAME	= ft_pipex_bonus.c ft_init_value_bonus.c ft_path_bonus.c ft_possible_path_bonus.c ft_error_pipefork_bonus.c ft_error_cmd_bonus.c
OBJB_NAME	= ${BNS_NAME:.c=.o}
SRC			= $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ			= $(addprefix $(OBJ_PATH), $(OBJ_NAME))
BNS 		= $(addprefix $(BNS_PATH), $(BNS_NAME))
OBJB		= $(addprefix $(OBJB_PATH), $(OBJB_NAME))
NAME		= pipex
BONUS		= pipex_bonus

CC		= cc
RM		= rm -rf

CFLAGS	= -Wall -Wextra -Werror 

all:		${NAME}

${OBJ_PATH}%.o: ${SRC_PATH}%.c
				@mkdir -p ${OBJ_PATH}
				${CC} ${CFLAGS} -c $< -o $@

${NAME}:	lib ${OBJ}
			@${CC} ${CFLAGS} ${OBJ} ./libft/libft.a -o ${NAME}

bonus:	${BONUS}

${OBJB_PATH}%.o:	${BNS_PATH}%.c
					@mkdir -p ${OBJB_PATH}
					${CC} ${CFLAGS} -c $< -o $@

${BONUS}:	lib ${OBJB}
			@${CC} ${CFLAGS} ${OBJB} ./libft/libft.a -o ${BONUS}

lib:
		@make -C libft

clean:
			@make clean -C ./libft/
			${RM} ${OBJ}
			${RM} ${OBJ_PATH}
			${RM} ${OBJB}
			${RM} ${OBJB_PATH}

fclean:		clean
					@make fclean -C ./libft/
					${RM} ${NAME}
					${RM} ${BONUS}

re:					fclean all

reb:				fclean bonus

.PHONY : all clean fclean re
