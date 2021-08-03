NAME = philo
CC = gcc
# INCLUDES =  -I ./includes/ -I ./libft/
# LIBS_ADD =  -L ./libft/ -lft 
CFLAGS = -Wall -Wextra -Werror ${INCLUDES} -pthread
SRCS =  main.c utils.c
OBJS = $(SRCS:.c=.o)

all: ${NAME}
debug:      CFLAGS  += -fsanitize=address -g
debug:      fclean all 

$(NAME) :${OBJS} 
		${CC} ${CFLAGS} -o ${NAME} ${OBJS} 
# tools:
# 	make bonus -C ./libft/
clean:
		rm -f ${OBJS}

fclean: clean
		rm -f ${NAME}
re: fclean all


