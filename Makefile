# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2019/10/08 11:37:09 by nerahmou     #+#   ##    ##    #+#        #
#    Updated: 2019/10/17 16:25:23 by nerahmou    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all, clean, fclean, re

.SUFFIXES:

red=$(echo -e "\033[0;31m")
ccyellow=$(echo -e "\033[0;33m")
ccend=$(echo -e "\033[0m")
green = $(@echo "\033[92m")
########################### VARS ############################

RM += -r
CC = gcc
DEBUG = 0

ifeq ($(DEBUG),1)
	CFLAGS = -g3 -Wall -Wpadded #-fsanitize=address
else
	CFLAGS = -Wall -Wextra -Werror -Wpadded -fsanitize=address
endif

HEADERS = malloc.h
HEADERS_DIR = includes


SRCS = main.c\
	   malloc.c
SRCS_DIR = srcs

OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))
OBJS_DIR = obj

LIBFT = libft.a
LIBFT_DIR = libft

NAME = test

#DIRS TO SEARCH IN IF ANY FILE NOT FOUND IN CURRENT DIRECTORY
#vpath %.a $(LIBFT_DIR)
vpath %.c $(SRCS_DIR)
vpath %.h $(HEADERS_DIR)




all: $(NAME)

$(NAME): libft/libft.a $(OBJS)
	@echo Link object files
	$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft $^ -o $(NAME)

$(OBJS_DIR)/%.o: %.c $(HEADERS) | $(OBJS_DIR)
	@echo Compile $< in $@
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -I$(LIBFT_DIR)/$(HEADERS_DIR) -c $< -o $@ 

libft/libft.a:
	make -C $(LIBFT_DIR)


$(OBJS_DIR):
	@echo Create $(OBJS_DIR) directory
	@mkdir $(OBJS_DIR)

clean:
	@echo "Appel de clean (LIBFT)"; make -C $(LIBFT_DIR) clean
	@$(RM) -rf $(OBJS_DIR)


fclean: clean
	@echo "Appel de fclean (LIBFT)"; make -C $(LIBFT_DIR) fclean
	@$(RM) -rf $(NAME)

re: fclean all
