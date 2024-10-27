NAME	=	philo
CC	=	cc
CFLAGS	=	-Wall -Werror -Wextra -g
HEADER = src/philo.h

SRCS_DIR = src
OBJ_DIR = obj

SRCS = $(addprefix $(SRCS_DIR)/, main.c input_check.c init.c actions.c threads.c threads_utils.c utils.c error.c)

OBJ = $(patsubst $(SRCS_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
BONUS_OBJ = $(BONUS_SRCS:.c=.o)

GREEN = \033[32m
RESET = \033[0m

all:	$(NAME)
	@echo "$(GREEN)$(NAME) compilation complete!$(RESET)"

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME)	$(OBJ)

# $(OBJ):	%.o:	%.c
# 	$(CC)	$(CFLAGS)	-c	$<	-o	$@

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER)| $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm	-f	$(OBJ)
	rm	-rf $(OBJ_DIR)
#remove only object files, not executable

fclean:	clean
	rm	-f	$(NAME)
	@echo "$(GREEN)Fully cleaned!$(RESET)"

re:	fclean	all
# run clean and recompile

.PHONY:	all	clean	fclean	re	bonus
#if you have files with these names then still run the command
#"-o" naming it