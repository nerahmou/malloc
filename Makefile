# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2019/10/08 11:37:09 by nerahmou     #+#   ##    ##    #+#        #
#    Updated: 2020/01/06 14:37:54 by nerahmou    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all, clean, fclean, re
.SUFFIXES:

########################### VARS ############################
bold=`tput bold`
normal=`tput sgr0`

DEBUG := 0
CC := cc
RM += -r

ifeq ($(DEBUG),1)
	CFLAGS := -g3 -O2 -Wall -Wextra -Wpadded -fPIC
else
	CFLAGS := -O2 -Wall -Wextra -Werror -Wpadded -fPIC
endif
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LFTDIR = ./libft/
LIB=$(LFTDIR)libft.a


HEADERS_DIR := includes
LIB_HEADER := libft/includes/
SRCS_DIR := srcs
OBJS_DIR := obj

HEADERS := $(addprefix $(HEADERS_DIR)/, malloc.h free.h)
SRCS := $(addprefix $(SRCS_DIR)/,	malloc.c\
									realloc.c\
									calloc.c\
									free.c\
									region.c\
									utils.c\
									defrag.c\
									bins.c\
									show_alloc_mem.c)
OBJS := $(addprefix $(OBJS_DIR)/,$(notdir $(SRCS:.c=.o)))

NAME := libft_malloc_$(HOSTTYPE).so
LINK_NAME := libft_malloc.so


all: build_libft $(NAME)

$(NAME): $(LIB) $(OBJS)
	@echo "\n${bold}Link object files${normal}"
	$(CC) -shared $(CFLAGS) $^ -L $(LFTDIR) -lft -o $(NAME)
	@echo "\n${bold}Symbolic link${normal}"
	ln -s $(NAME) $(LINK_NAME) 2> /dev/null || true

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADERS) | $(OBJS_DIR)
	@echo "\n${bold}Compile $< in $@${normal}"
	$(CC) $(CFLAGS) -I $(HEADERS_DIR) -I $(LIB_HEADER) -c $< -o $@ 

$(OBJS_DIR):
	@echo "${bold}Create $@ directory${normal}"
	mkdir $@

build_libft:
	@make -C $(LFTDIR)

clean:
	@echo "${bold}Clean${normal}"
	$(RM) $(OBJS_DIR)

fclean: clean
	@echo "${bold}Fclean${normal}"
	$(RM) $(NAME) $(LINK_NAME)
	make -C $(LFTDIR) fclean

re: fclean all
