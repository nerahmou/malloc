# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2019/10/08 11:37:09 by nerahmou     #+#   ##    ##    #+#        #
#    Updated: 2019/10/08 16:12:06 by nerahmou    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all, clean, fclean, re

.SUFFIXES:


########################### VARS ############################

RM += -r
CC = gcc

ifeq ($(DEBUG),1)
	CFLAGS = -g3 -Wall -Wextra -Werror
else
	CFLAGS = -Wall -Wextra -Werror
endif

HEADERS = test.h
HEADERS_DIR = includes

SRCS = salut.c main.c
SRCS_DIR = srcs

OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))
OBJS_DIR = obj

NAME = test

#DIRS TO SEARCH IN IF ANY FILE NOT FOUND IN CURRENT DIRECTORY
vpath %.c $(SRCS_DIR)
vpath %.h $(HEADERS_DIR)




all: $(NAME)

$(NAME): $(OBJS)
	@echo Link object files
	@$(CC) $(CFLAGS) $^ -o $(NAME)

$(OBJS_DIR)/%.o: %.c $(HEADERS) | $(OBJS_DIR)
	@echo Compile $< in $@
	@$(CC) $(CFLAGS) -I$(HEADERS_DIR) -c $< -o $@ 

$(OBJS_DIR):
	@echo Create $(OBJS_DIR) directory
	@mkdir $(OBJS_DIR)
clean:
	@$(RM) -rf $(OBJS_DIR)

fclean: clean
	@$(RM) -rf $(NAME)

re: fclean all
