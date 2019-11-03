//
//  ConnectTable.cpp
//  
//
//  Created by Calvin Huang on 10/18/19.
//

#include "ConnectTable.hpp"
ConnectTable::ConnectTable()
{
    game_over = false;
    turn = 0;
    for(int i = 0; i < row_; i++)
    {
        for (int j = 0; j < column_; j++)
        {
            board[i][j] = '-';
        }
    }
    for (int i = 0; i < column_; i++)
    {
        cur_row.push_back(5);
    }
}
ConnectTable::ConnectTable(const ConnectTable & rhs)
{
    game_over = rhs.game_over;
    AI_PIECE = rhs.AI_PIECE;
    P_PIECE = rhs.P_PIECE;
    for (int i = 0; i < row_; i++)
    {
        for (int j = 0; j < column_; j++)
        {
            board[i][j] = rhs.board[i][j];
        }
    }
    for (int i = 0; i < rhs.cur_row.size(); i++)
    {
        cur_row.push_back(rhs.cur_row[i]);
    }
}
bool ConnectTable::Check(char piece)
{
    //check horizontal
    for(int i = 0; i < row_; i++)
    {
        for (int j = 0; j < column_ - 3; j++)
        {
            if (board[i][j] == piece && board[i][j + 1] == piece && board[i][j + 2] == piece && board[i][j + 3] == piece) return true;
        }
    }
    //check vert
    for (int i = 0; i < row_ - 3; i++)
    {
        for (int j = 0; j < column_; j++)
        {
            if (board[i][j] == piece && board[i + 1][j] == piece && board[i + 2][j] == piece && board[i + 3][j] == piece)return true;
        }
    }
    //check diag right
    for (int i = 0; i < row_ - 3; i++)
    {
        for (int j = 0; j < column_ - 3; j++)
        {
            if (board[i][j] == piece && board[i + 1][j + 1] == piece && board[i + 2][j + 2] == piece && board[i + 3][j + 3] == piece)return true;
        }
    }
    //check diag left
    for (int i = 3; i < row_; i++)
    {
        for (int j = 0; j < column_ - 3; j++)
        {
            if (board[i][j] == piece && board[i - 1][j + 1] == piece && board[i - 2][j + 2] == piece && board[i - 3][j + 3] == piece)return true;
        }
    }
    return false;
}
int ConnectTable::score_position(char piece)
{
    int score = 0;
    //column score
    for (int i = 0; i < row_; i++)
    {
        if (board[row_][column_/2] == piece) score += 3;
    }
    //horizontal eval
    for (int i = 0; i < row_; i++)
    {
        for (int j = 0; j < column_ - 3; j++)
        {
            std::vector<char> sub;
            for (int k = j; k < j + 4; k++)
                sub.push_back(board[i][k]);
            score += evaluate_window(sub, piece);
        }
    }
    //vertical
    for (int i = 0; i < row_ - 3; i++)
    {
        for (int j = 0; j < column_; j++)
        {
            std::vector<char> sub;
            for (int k = i; k < i + 4; k++)
            {
                sub.push_back(board[k][j]);
                score += evaluate_window(sub, piece);
            }
        }
    }
    //diag right
    for (int i = 0; i < row_ - 3; i++)
    {
        for (int j = 0; j < column_ - 3; j++)
        {
            std::vector<char> sub;
            for (int k = 0; k < 4; k++)
            {
                sub.push_back(board[i + k][j + k]);
            }
            score += evaluate_window(sub, piece);
        }
    }
    //diag left
    for (int i = 3; i < row_; i++)
    {
        for (int j = 0; j < column_ - 3; j++)
        {
            std::vector<char> sub;
            for (int k = 0; k < 4; k++)
            {
                sub.push_back(board[row_ - k][column_ + k]);
            }
            score += evaluate_window(sub, piece);
        }
    }
    //std::cout << score <<std::endl;
    return score;
}
int ConnectTable::evaluate_window(std::vector<char> windowofrow, char piece)
{
    int score = 0;
    char opppiece = 'O';
    if (piece == 'O') opppiece = 'X';
    if (count(windowofrow.begin(), windowofrow.end(), piece) == 4)score += 100;
    else if (count(windowofrow.begin(), windowofrow.end(), piece) == 3 && count(windowofrow.begin(), windowofrow.end(), '-') == 1) score += 5;
    else if (count(windowofrow.begin(), windowofrow.end(), piece) == 2 && count(windowofrow.begin(), windowofrow.end(), '-') == 2) score += 2;
    else if (count(windowofrow.begin(), windowofrow.end(), opppiece) == 3 && count(windowofrow.begin(), windowofrow.end(), '-') == 1) score -= 8;
    return score;
}
void ConnectTable::Play()
{
    AI = 0;
    Player = 1;
    AI_PIECE = 'O';
    P_PIECE = 'X';
    while (!game_over)
    {
        if (turn == Player)
        {
            int col;
            std::cout << "Pick column" << std::endl;
            std::cin >> col;
            if (IsValidLocation(col))
            {
                DropPiece(col, P_PIECE);
                turn += 1;
                turn %= 2;
                if (Check(P_PIECE))
                {
                    game_over = true;
                }
            }
        }
        else if (turn == AI)
        {
            std::pair<int, int> c = minimax(5, INT_MIN, INT_MAX, true);
            if (IsValidLocation(c.first))
            {
                DropPiece(c.first, AI_PIECE);
                turn += 1;
                turn %= 2;
                if (Check(AI_PIECE))
                {
                    game_over = true;
                }
            }
        }
        print();
    }
}

bool ConnectTable::IsValidLocation(int col)
{
    if(col > column_ || col < 0)return false;
    if (cur_row[col] > row_ || cur_row[col]  < 0)return false;
    return true;
}

bool ConnectTable::DropPiece(int col, char pieces)
{
    if (IsValidLocation(col))
    {
        board[cur_row[col]][col] = pieces;
        cur_row[col] = --cur_row[col];
        return true;
    }
    return false;
}
bool ConnectTable::terminalnode()
{
    std::vector<int> numcol = validlocations();
    if (Check(AI_PIECE) || Check(P_PIECE) || numcol.size() == 0) return true;
    return false;
}
std::vector<int> ConnectTable::validlocations()
{
    std::vector<int> locations;
    for (int i = 0; i < column_; i++)
    {
        if (IsValidLocation(i))locations.push_back(i);
    }
    return locations;
}
std::pair<int,int> ConnectTable::minimax(int depth, int alpha, int beta, bool maximizingplayer)
{
    std::vector<int> validloc_ = validlocations();
    if (terminalnode())
    {
        if (Check(AI_PIECE))return std::pair<int, int>(-1, 1000000000);
        else if (Check(P_PIECE))return std::pair<int, int>(-1, -100000000);
        else  return std::pair<int, int>(-1, 0);
    }
    else if (depth == 0)
        return std::pair<int,int>(-1, score_position(AI_PIECE));
    if (maximizingplayer)
    {
        int value = INT_MIN;
        int columnn;
        if (validloc_.size() == 0)columnn = 0;
        else columnn = validloc_[0];
        for (int i = 0; i < validloc_.size(); i++)
        {
            ConnectTable c = *this;
            c.DropPiece(i, AI_PIECE);
            std::pair<int, int> new_score = c.minimax(depth - 1, alpha, beta, false);
            if (new_score.second > value)
            {
                value = new_score.second;
                columnn = i;
            }
            alpha = std::max(alpha, value);
            if (alpha >= beta)break;
        }
        return std::pair<int, int>(columnn, value);
    }
    else
    {
        int value = INT_MAX;
        int columnn;
        if (validloc_.size() == 0)columnn = 0;
        else columnn = validloc_[0];
        for (int i = 0; i < validloc_.size(); i++)
        {
            ConnectTable c = *this;
            c.DropPiece(i, P_PIECE);
            std::pair<int, int> new_score = c.minimax(depth - 1, alpha, beta, true);
            if (new_score.second < value)
            {
                value = new_score.second;
                columnn = i;
            }
            beta = std::min(beta,value);
            if (alpha >= beta)break;
        }
        return std::pair<int, int>(columnn, value);
    }
}

void ConnectTable::print()
{
    for (int i = 0; i < row_; i++)
    {
        for (int j = 0; j < column_; j++)
        {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
