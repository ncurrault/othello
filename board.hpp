#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <cstdint>
#include "common.hpp"
#include <iostream>
using namespace std;

class Board {

private:
    uint_fast64_t black;
    uint_fast64_t taken;

    bool bitstringGet(uint_fast64_t bitstring, int i);
    bool bitstringGet(uint_fast64_t bitstring, int x, int y);
    void bitstringSet(uint_fast64_t& bitstring, int i, bool val);
    void bitstringSet(uint_fast64_t& bitstring, int x, int y, bool val);
    int bitstringCount(uint_fast64_t bitstring);

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
    int getStableCells(Side side);

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);

    int getValue(Side side);
    int getNaiveValue(Side side);
};

#endif
