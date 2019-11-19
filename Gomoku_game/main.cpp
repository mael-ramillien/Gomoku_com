/*
** EPITECH PROJECT, 2019
** AIA_gomoku_2019
** File description:
** main
*/

#include "board.h"

using namespace std;

int main()
{
    bool end = true;
    bool error = false;
    int fill[3];
    board my_board;
    fill[2] = 1;
    cout << "\x1B[2J\x1B[H" << "Tour du joueur 1 => X:\n";
    while(end){
        error = false;
        my_board.display_board();
        cout << "posx(0-18)/posy(0-18):" << endl;
        cin >> fill[0] >> fill[1];
        if (my_board.check_cell(fill[0], fill[1])){
            my_board.fill_cell(fill[0], fill[1], fill[2]);
        } else {
            error = true;
            if (fill[2] == 1){
                fill[2] = 2;
            } else {
                fill[2] = 1;
            }
        }







        if (my_board.check_victory(fill[0], fill[1], fill[2])){
            end = false;
            cout << "\x1B[2J\x1B[H";
            my_board.display_board();
            if (fill[2] == 1){
                cout  << "le gagnant est joueur 1 => X\n";
            }
            else {
                cout  << "le gagnant est joueur 2 => O\n";
            }
        }
        if (fill[2] == 1 && end){
            fill[2] = 2;
        cout << "\x1B[2J\x1B[H" << "Tour du joueur 2 => O:\n";
        }
        else if (end){
            fill[2] = 1;
        cout << "\x1B[2J\x1B[H" << "Tour du joueur 1 => X:\n";
        }
        if (error && end)
            cout << "Can't put pawn there\n";
    }
    return 0;
}