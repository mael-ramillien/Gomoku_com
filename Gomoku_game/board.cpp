/*
** EPITECH PROJECT, 2019
** AIA_gomoku_2019
** File description:
** board
*/

#include <stdlib.h>
#include "board.h"

using namespace std;

board::board(){
    for (int i = 0;i < 19;i++)
        for(int j = 0;j< 19;j++)
            tab[i][j] = 0;
}

int board::get_cell(int posx, int posy){
    return tab[posx][posy];
}

bool board::check_victory(int posx, int posy, int value){
    int compteur = 0;
    bool end = true;
    int temp = posy-4;
    if (temp < 0)
        temp = 0;
    while (end){
        if (tab[posx][temp] == value)
            compteur++;
        else if (compteur > 0)
            compteur--;
        if (compteur == 5)
            return true;
        if (temp == posy+4)
            end = false;
        temp++;    
    }

    compteur = 0;
    end = true;
    temp = posx-4;
    if (temp < 0)
        temp = 0;
    while (end){
        if (tab[temp][posy] == value)
            compteur++;
        else if (compteur > 0)
            compteur--;
        if (compteur == 5)
            return true;
        if (temp == posx+4)
            end = false;
        temp++;    
    }

    compteur = 0;
    end = true;
    temp = posx-4;
    int temp2 = posy-4;
    if (temp < 0)
        temp = 0;
    if (temp2 < 0)
        temp2 = 0;
    while (end){
        if (tab[temp][temp2] == value)
            compteur++;
        else if (compteur > 0)
            compteur--;
        if (compteur == 5)
            return true;
        if (temp == posx+4)
            end = false;
        temp++;
        temp2++; 
    }

    compteur = 0;
    end = true;
    temp = posx+4;
    temp2 = posy-4;
    if (temp < 0)
        temp = 0;
    if (temp2 < 0)
        temp2 = 0;
    while (end){
        if (tab[temp][temp2] == value)
            compteur++;
        else if (compteur > 0)
            compteur--;
        if (compteur == 5)
            return true;
        if (temp == posx-4)
            end = false;
        temp--;
        temp2++;
    }
    return false;
}

bool board::check_cell(int posx, int posy){
    if (tab[posx][posy] == 0)
        return true;
    return false;
}

void board::fill_cell(int posx, int posy, int value){
    if (check_cell(posx, posy))
        tab[posx][posy] = value;
}

void board::display_line(int line[19])
{
    cout << "|";
    for (int i = 0;i< 19;i++){
        cout << " ";
        if (line[i] == 0)
            cout << ".";
        if (line[i] == 1)
            cout << "X";
        if (line[i] == 2)
            cout << "O";
    }
    cout << " |\n";
}

void board::display_board()
{
    cout << "- - - - - - - - - - - - - - - - - - - - -\n";
    for (int i = 0;i < 19;i++)
        display_line(tab[i]);
    cout << "- - - - - - - - - - - - - - - - - - - - -\n";
}
