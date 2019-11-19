/*
** EPITECH PROJECT, 2019
** AIA_gomoku_2019
** File description:
** board
*/

#ifndef BOARD_H_
#define BOARD_H_

#include <iostream>

class board
{
    public:

    board();
    void fill_cell(int posx, int posy, int value);
    void display_board();
    void display_line(int line[19]);
    bool check_cell(int posx, int posy);
    int get_cell(int pox, int posy);
    bool check_victory(int posx, int posy, int value);
    
    private:

    int tab[19][19]; //0 case vide // 1 case joueur 1 => X // 2 case joueur 2 => O //
};

#endif /* !BOARD_H_ */
