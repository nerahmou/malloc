# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2019/10/08 11:37:09 by nerahmou     #+#   ##    ##    #+#        #
#    Updated: 2019/11/25 11:39:07 by nerahmou    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all, clean, fclean, re, main_test
.SUFFIXES:

########################### VARS ############################
DEBUG := 0
CC := cc
RM += -r

ifeq ($(DEBUG),1)
	CFLAGS := -Wall -Wextra -Wpadded -fPIC
else
	CFLAGS := -g -Wall -Wextra -Werror -Wpadded -fPIC -pedantic -Wcast-align\
		-Wcast-qual -Wdisabled-optimization -Wformat=2\
		-Winit-self -Wuninitialized -Wmissing-include-dirs\
		-Wredundant-decls -Wshadow\
		-fstrict-overflow -Wstrict-overflow=5 -Wundef -Wno-unused\
		-Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
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
									utils.c\
									show_alloc_mem.c)
OBJS := $(addprefix $(OBJS_DIR)/,$(notdir $(SRCS:.c=.o)))

NAME := libft_malloc_$(HOSTTYPE).so
LINK_NAME := libft_malloc.so


all: $(NAME)

$(NAME): $(LIB) $(OBJS)
	@echo "Link object files"
	$(CC) -shared $(CFLAGS) $^ -L $(LFTDIR) -lft -o $(NAME)
	ln -s $(NAME) $(LINK_NAME) 2> /dev/null || true

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADERS) | $(OBJS_DIR)
	@echo "Compile $< in $@"
	$(CC) $(CFLAGS) -I $(HEADERS_DIR) -I $(LIB_HEADER) -c $< -o $@ 

$(OBJS_DIR):
	@echo "Create $@ directory"
	mkdir $@

$(LIB):
	make -C $(LFTDIR)

clean:
	@echo "Clean"
	$(RM) $(OBJS_DIR)

fclean: clean
	@echo "Fclean"
	$(RM) $(NAME) $(LINK_NAME)
	make -C $(LFTDIR) fclean

re: fclean all
