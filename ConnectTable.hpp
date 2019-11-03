//
//  ConnectTable.hpp
//  
//
//  Created by Calvin Huang on 10/18/19.
//
#include <iostream>
#include <vector>
#include <algorithm>
#ifndef ConnectTable_hpp
#define ConnectTable_hpp

class ConnectTable{
public:
    ConnectTable();
    ConnectTable(const ConnectTable & rhs);
    bool DropPiece(int col, char piece);
    bool IsValidLocation(int col);
    bool Check(char piece);
    void Play();
    int evaluate_window(std::vector<char> windowofrow,char piece);
    int score_position(char piece);
    bool terminalnode();
    std::vector<int> validlocations();
    std::pair<int,int> minimax(int depth, int alpha, int beta, bool maximizingplayer);
    void print();
private:
    const int column_ = 7;
    const int row_ = 6;
    char board[6][7];
    std::vector<int> cur_row;
    bool game_over;
    int turn;
    int AI;
    int Player;
    char AI_PIECE;
    char P_PIECE;
};


#endif /* ConnectTable_hpp */
