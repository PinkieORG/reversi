#include "reversi.hpp"

coordinate::coordinate(int x, int y, int z)
            : _x(x), _y(y), _z(z) {}

int coordinate::x() const { return _x; }

int coordinate::y() const { return _y; }

int coordinate::z() const { return _z; }

coordinate coordinate::operator + (coordinate other) {
    return coordinate(_x + other.x(), _y + other.y(), _z + other.z());
}

bool coordinate::operator < (int other) {
    return (_x < other && _y < other && _z < other);
}

bool coordinate::operator > (int other) {
    return (_x > other && _y > other && _z > other);
}

std::vector<coordinate> coordinate::neighbours() {
    std::vector<coordinate> output;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                output.emplace_back(i, j, k);
            }
        }
    }
    return output;
}

square::square() = default;

square::square(enum colour colour)
            :   _colour(colour) {}

enum colour square::colour() const { return _colour; }

void square::setColour(enum colour colour) { _colour = colour; }

void square::flip() {

    _colour == white ? _colour = black : _colour = white;
}

board::board(unsigned size)
        :   _size(size) {

    std::vector<std::vector<std::vector<square>>> board(_size, std::vector<std::vector<square>>(_size, std::vector<square>(_size)));
    _board = board;


    getSquare(transform(1, 1, 1)).setColour(white);
    getSquare(transform(1, 1, -1)).setColour(black);
    getSquare(transform(1, -1, -1)).setColour(white);
    getSquare(transform(1, -1, 1)).setColour(black);
    getSquare(transform(-1, 1, 1)).setColour(black);
    getSquare(transform(-1, 1, -1)).setColour(white);
    getSquare(transform(-1, -1, -1)).setColour(black);
    getSquare(transform(-1, -1, 1)).setColour(white);
}

bool board::gameLoop(int x, int y, int z, colour player, colour opponent) {

    auto newCoord = coordinate(transform(x, y, z));
    if (!isOnBoard(newCoord) || getSquare(newCoord).colour() != none) {
        return false;
    }
    bool valid = false;

    std::vector<square *> toFlip;

    for(coordinate n: coordinate::neighbours()) {
        coordinate lineCoord = newCoord + n;
        if (!isOnBoard(lineCoord) || getSquare(lineCoord).colour() != opponent) {
            continue;
        }


        toFlip.clear();

        while (isOnBoard(lineCoord) && getSquare(lineCoord).colour() == opponent) {
            toFlip.push_back(&getSquare(lineCoord));
            lineCoord = lineCoord + n;
        }


        if (!isOnBoard(lineCoord) || getSquare(lineCoord).colour() != player) {
            continue;
        }
        valid = true;
        getSquare(newCoord).setColour(player);
        for (square *s: toFlip) {
            (*s).flip();
        }
    }
    return valid;
}

coordinate board::transform(int x, int y, int z) {

    (x < 0) ? x += _size / 2 : x += _size / 2 - 1;
    (y < 0) ? y += _size / 2 : y += _size / 2 - 1;
    (z < 0) ? z += _size / 2 : z += _size / 2 - 1;
    return coordinate(x, y, z);
}

square& board::getSquare(coordinate coord) {
    return _board[coord.x()][coord.y()][coord.z()];
}

bool board::isOnBoard(coordinate coord) const {
    return (coord < _size && coord > -1);
}

bool board::isFull() const {
    for (const auto& x: _board) {
        for (const auto& y: x) {
            for (square s: y) {
                if (s.colour() == none) {
                    return false;
                }
            }
        }
    }
    return true;
}

int board::countScore() const {
    int score = 0;

    for (const auto& x: _board) {
        for (const auto& y: x) {
            for (square s: y) {
                s.colour() == white ? ++score : --score;
            }
        }
    }
    return score;
}

reversi::reversi(int size)
        :   _board(board(size)),
            _player(white),
            _opponent(black)
    {}

void reversi::swapPlayers() {
    std::swap(_player, _opponent);
}

bool reversi::play(int x, int y, int z) {
    if (x == 0 && y == 0 && z == 0) {
        swapPlayers();
        return true;
    }
    bool valid = _board.gameLoop(x, y, z, _player, _opponent);
    if (valid) {
        swapPlayers();
    }
    return valid;
}

bool reversi::finished() const { return _board.isFull(); }

int reversi::result() const { return _board.countScore(); }

int main() {

}
 
