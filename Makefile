# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2019/10/08 11:37:09 by nerahmou     #+#   ##    ##    #+#        #
#    Updated: 2019/10/19 17:02:25 by nerahmou    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all, clean, fclean, re, malloc
.SUFFIXES:

########################### VARS ############################
#SHELL := /bin/zsh
DEBUG := 0
CC := gcc
RM += -r

ifeq ($(DEBUG),1)
	CFLAGS := -g3 -Wall -Wpadded -fPIC -fsanitize=address
else
	CFLAGS := -Wall -Wextra -Werror -Wpadded -fPIC -fsanitize=address
endif
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif


HEADERS_DIR := includes
SRCS_DIR := srcs
OBJS_DIR := obj

HEADERS := $(addprefix $(HEADERS_DIR)/,malloc.h)
SRCS := $(addprefix $(SRCS_DIR)/,malloc.c)
OBJS := $(addprefix $(OBJS_DIR)/,$(notdir $(SRCS:.c=.o)))

NAME := libft_malloc_$(HOSTTYPE).so
LINK_NAME := libft_malloc.so


all: $(NAME)

$(NAME): $(OBJS)
	@echo "Link object files" 1>&2
	$(CC) -shared $(CFLAGS) $^ -o $(NAME)
	ln -s $(NAME) $(LINK_NAME) 2> /dev/null || true

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADERS) | $(OBJS_DIR)
	@echo "Compile $< in $@" 1>&2
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -c $< -o $@ 

$(OBJS_DIR):
	@echo "Create $@ directory" 1>&2
	mkdir $@

clean:
	@echo "Clean" 1>&2
	$(RM) $(OBJS_DIR)

fclean: clean
	@echo "Fclean" 1>&2
	$(RM) $(NAME)

re: fclean all

init_env:
	@printf "export DYLD_LIBRARY_PATH='.' DYLD_INSERT_LIBRARIES='libft_malloc.so'"