#include "board.hpp"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
int Board::getNaiveValue(Side side){
    int score = countBlack()-countWhite();
    if(side==Side::WHITE){
        score=-score;
    }
    return score;
}
int Board::getValue(Side side){
    int score = countBlack()-countWhite();
    if(side==Side::WHITE){
        score=-score;
    }
    //corners: 00 07 70 77
    //inners:  11 16 61 66
    int outer[2] = {0,7};
    int inner[2] = {1,6};
    int stable=getStableCells(side);

    score+=2*stable;
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            if(get(side,outer[i],outer[j])){
                score*=3;
            }
            else if(get(side,inner[i],inner[j])){
                score/=2;
            }
        }
    }

    return score;
}

int Board::getStableCells(Side side){
    bitset<64> stable;
    //things stable from top left
    for(int y=0; y<8; y++){
        for(int x=0; x<8; x++){
            if(!get(side,x,y))
                break;
            if(y!=0 && !stable[x+8*(y-1)])
                break;
            stable.set(x+8*y);
        }
    }
    //things stable from top right
    for(int y=0; y<8; y++){
        for(int x=7; x>=0; x--){
            if(!get(side,x,y))
                break;
            if(y!=0 && !stable[x+8*(y-1)])
                break;
            stable.set(x+8*y);
        }
    }
    //things stable from bottom right
    for(int y=7; y>=0; y--){
        for(int x=7; x>=0; x--){
            if(!get(side,x,y))
                break;
            if(y!=7 && !stable[x+8*(y+1)])
                break;
            stable.set(x+8*y);
        }
    }
    //things stable from bottom left
    for(int y=7; y>=0; y--){
        for(int x=0; x<8; x++){
            if(!get(side,x,y))
                break;
            if(y!=7 && !stable[x+8*(y+1)])
                break;
            stable.set(x+8*y);
        }
    }
    int stable_sum=0;
    for(int i=0; i<64; i++){
        stable_sum+=stable[i];
    }
    return stable_sum;
}

int Board::lossQuadrantValue(int quadrant, bool considerVert, int numVert, bool considerHoriz, int numHoriz) {
    int xStart = 0;
    int yStart = 0;
    if (quadrant == 3 or quadrant == 4) {
        yStart = 4;
    }
    if (quadrant == 2 || quadrant == 4){
        xStart = 4;
    }

    std::vector<int> colSums( {0, 0, 0, 0} );
    std::vector<int> rowSums( {0, 0, 0, 0} );

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int occ = occupied(xStart + i, yStart + j);
            int color = get(Side::WHITE, xStart + i, yStart + j);

            rowSums[i] += color;
            colSums[j] += color;
        }
    }

    int val = 0;
    if (considerHoriz) {
        while (numHoriz > 0) {
            int max = INT_MIN;
            int maxI = -1;
            for (unsigned int i = 0; i < rowSums.size(); i++) {
                if (rowSums[i] > max) {
                    max = rowSums[i];
                    maxI = i;
                }
            }
            val += max;
            rowSums.erase(rowSums.begin() + maxI);

            numHoriz--;
        }
        for (int n : rowSums) {
            val -= n;
        }
    }
    if (considerVert) {
        while (numVert > 0) {
            int max = INT_MIN;
            int maxI = -1;
            for (unsigned int i = 0; i < colSums.size(); i++) {
                if (colSums[i] > max) {
                    max = colSums[i];
                    maxI = i;
                }
            }
            val += max;
            colSums.erase(colSums.begin() + maxI);

            numVert--;
        }
        for (int n : colSums) {
            val -= n;
        }
    }

    if (considerVert && considerHoriz) {
        val /= 2; // maybe unnecessary?
    }

    return val;
}
int Board::getLossJpgValue() {
    return lossQuadrantValue(1, true, 1, false, 0)
        +  lossQuadrantValue(2, true, 2, false, 0)
        +  lossQuadrantValue(3, true, 2, false, 0)
        +  lossQuadrantValue(4, true, 1, true , 1);
}
