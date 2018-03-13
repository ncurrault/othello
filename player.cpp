#include "player.hpp"

#define TEST_MINIMAX_DEPTH 2
#define FULL_MINIMAX_DEPTH 5

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    playerSide=side;
    opponentSide=playerSide==Side::BLACK ? Side::WHITE : Side::BLACK;
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    b.doMove(opponentsMove,opponentSide);

    Move *m = nullptr;
    int alpha = INT_MIN;
    int beta = INT_MAX;

    if (testingMinimax) {
        tryMove(b, m, playerSide, false, alpha, beta, TEST_MINIMAX_DEPTH);
    } else {
        tryMove(b, m, playerSide, false, alpha, beta, FULL_MINIMAX_DEPTH);
    }

    if (m != nullptr) {
        b.doMove(m, playerSide);
    }

    return m;
}

/**
 * Get the value of a given move by analyzing `depth` levels of the minimax tree
 * If move is nullptr, all moves are tried and move is set to the optimal one
 * (or leave move as nullptr if there are no valid moves)
 */
int Player::tryMove(Board board, Move*& move, Side side, bool isOpponent, int& alpha, int& beta, int depth) {
    if (move == nullptr) {
        /* if move is null, we want to first optimize the move,
        so the "other side" to optimize is actually the player side */
        side = (side == Side::BLACK) ? Side::WHITE : Side::BLACK;
    } else {
        board.doMove(move, side);
    }

    if (depth == 0) {
        if (testingMinimax) {
            return board.getNaiveValue(side);
        } else {
            return board.getValue(side);
        }
    } else {
        Side other = (side == Side::BLACK) ? Side::WHITE : Side::BLACK;
        Move* oppMove = new Move(0, 0);
        Move* best = nullptr;
        int maxSoFar = INT_MIN;

        for(int i = 0; i < 8; i++) {
            oppMove->setX(i);
            for(int j = 0; j < 8; j++) {
                oppMove->setY(j);
                if(board.checkMove(oppMove, other)){
                    int newVal = tryMove(board, oppMove, other, depth - 1);
                    if (best == nullptr) {
                        best = new Move(i,j);
                        maxSoFar = newVal;
                    } else if (newVal > maxSoFar) {
                        best->setX(i);
                        best->setY(j);
                        maxSoFar = newVal;
                    }
                }
            }
        }

        delete oppMove;
        if (move == nullptr) {
            move = best; // return the best move using this reference param
            return maxSoFar;
        } else {
            delete best;
            return -maxSoFar;
        }
    }
}

void Player::setBoard(char data[]) {
    b.setBoard(data);
}
