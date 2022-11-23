# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/15 12:32:56 by mportrai          #+#    #+#              #
#    Updated: 2022/11/23 04:54:08 by tbousque         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	= ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c  ft_substr.c ft_tolower.c ft_toupper.c ft_strtrim.c ft_split.c ft_itoa.c ft_putchar_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_strmapi.c ft_striteri.c ft_strlen_l.c ft_strcmp.c ft_ullto_buf.c ft_strrev.c ft_strndup.c

SRCSB	= ft_lstnew.c ft_lstsize.c ft_lstlast.c ft_lstadd_front.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

OBJS	= ${SRCS:.c=.o}

OBJSB	= ${SRCSB:.c=.o}

NAME	= libft.a

CC		= cc #TODO: use cc
RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -g

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

all:		${NAME}

${NAME}: ${OBJS}
		ar rc ${NAME} ${OBJS}

bonus:	${OBJSB}
		ar rc ${NAME} ${OBJSB}
clean:
			${RM} ${OBJS} ${OBJSB}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY : all clean fclean re
