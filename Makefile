##
## EPITECH PROJECT, 2019
## AIA_gomoku_2019
## File description:
## Makefile
##

NAME = gomoku

SRC = example.cpp

all: $(NAME)

$(NAME):
	g++ -o $(NAME) $(SRC)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all