# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/13 14:02:20 by fesper-s          #+#    #+#              #
#    Updated: 2023/02/08 08:40:51 by fesper-s         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CFLAGS	= -g -Wall -Wextra -Werror

CPPFLAGS = -I /opt/homebrew/Cellar/readline/8.2.1/include

LDFLAGS = -L /opt/homebrew/Cellar/readline/8.2.1/lib

CPPFLAGS += -I ~/.brew/opt/readline/include

LDFLAGS += -L ~/.brew/opt/readline/lib

LIBFT	= ./libft/libft.a 

SRCS	= ./main.c ./error.c ./more_error.c ./signal.c ./utils.c \
		  ./minishell.c ./check.c ./init_struct.c ./list_utils.c ./builtins.c \
		  ./echo.c ./chdir.c ./export.c ./unset.c ./builtins_utils.c \
		  ./memory.c ./expand.c

OBJS	= $(SRCS:.c=.o)

$(NAME):	$(OBJS)
			make -C ./libft
			cc $^ $(CFLAGS) $(LIBFT) -o $@ -lreadline $(CPPFLAGS) $(LDFLAGS)

all:		$(NAME)

clean:
			rm -f $(OBJS)
			make clean -C ./libft

fclean:		clean
			rm -f $(NAME)
			rm -rf $(NAME).dSYM
			make fclean -C ./libft

re:			fclean all

.PHONY:		all clean fclean re
