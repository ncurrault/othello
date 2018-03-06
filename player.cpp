#include "player.hpp"

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
    Move *m =new Move(0,0);
    int best_score=INT_MIN;
    Move *best=nullptr;
    Board * new_board;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            m->setX(i);
            m->setY(j);
            if(b.checkMove(m, playerSide)){
                new_board =b.copy();
                new_board->doMove(m,playerSide);
                int val = new_board->getValue(playerSide);
                if(val>best_score){
                    if(best==nullptr){
                        best=new Move(i,j);
                    }
                    else{
                        best->setX(i);
                        best->setY(j);
                    }
                    best_score=val;
                }
                delete new_board;
            }
        }
    }
    b.doMove(best,playerSide);
    return best;
}

void Player::setBoard(char data[]) {
    b.setBoard(data);
}
