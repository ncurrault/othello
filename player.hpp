#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "climits"
using namespace std;

class Player {
private:
    Board b;
    Side playerSide;
    Side opponentSide;
public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    int tryMove(Board board, Move*& move, Side side, int depth);
    void setBoard(char data[]);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
