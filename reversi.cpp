#include "reversi.hpp"
#include <iostream>
#include <functional>

coordinate operator+(coordinate first, coordinate second) {
    coordinate result(first.size());
    for (int i = 0; i < first.size(); ++i) {
        result[i] = first[i] + second[i];
    }
    return result;
}

colour& opponent (colour& col) {
    return col = col == black ? white : black;
}

std::vector<coordinate> directions() {
    std::vector<coordinate> directions;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                directions.emplace_back(std::vector<int>{i, j, k});
            }
        }
    }
    return directions;
}

board::board(unsigned size)
        :   _size(size) {

    _boardData = boardData(_size, std::vector<std::vector<colour>>(_size, std::vector<colour>(_size)));

    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            for (int k = -1; k <= 1; k += 2) {
                colour col = (i * j * k) == 1 ? white : black;
                setColour(transform(i, j, k), col);
            }
        }
    }
}

bool board::makeMove(const coordinate& coord, colour player) {
    bool valid = false;

    if (!isOnBoard(coord) || getColour(coord) != none) {
        return valid;
    }

    for(const coordinate& dir: directions()) {
        std::vector<std::reference_wrapper<coordinate>> toFlip;

        coordinate lineCoord = coord + dir;
        while (isOnBoard(lineCoord) && getColour(lineCoord) == opponent(player)) {
            toFlip.emplace_back(lineCoord);
            lineCoord = lineCoord + dir;
        }

        if (!isOnBoard(lineCoord)) {
            continue;
        }

        if (!toFlip.empty() && getColour(lineCoord) == player) {
            valid = true;
            for (coordinate& coo: toFlip) {
                flip(coo);
            }
        }
    }
    return valid;
}

coordinate board::transform(int x, int y, int z) const {
    int halfSize = (int)_size / 2;

    x += (x < 0) ? halfSize : halfSize - 1;
    y += (y < 0) ? halfSize : halfSize - 1;
    z += (z < 0) ? halfSize : halfSize - 1;
    return {x, y, z};
}

bool board::isOnBoard(coordinate coord) const {
    bool result = true;
    for (size_t i = 0; i <= 2; ++i) {
        if (coord[i] < 0 || coord[i] >= _size) {
            result = false;
        }
    }
    return result;
}

bool board::isFull() const {
    for (const auto& x: _boardData) {
        for (const auto& y: x) {
            for (colour col: y) {
                if (col == none) {
                    return false;
                }
            }
        }
    }
    return true;
}

int board::countScore() const {
    int score = 0;

    for (const auto& x: _boardData) {
        for (const auto& y: x) {
            for (colour col: y) {
                col == white ? ++score : --score;
            }
        }
    }
    return score;
}

colour board::getColour(coordinate coord) {
    return _boardData[coord[0]][coord[1]][coord[2]];
}

void board::flip(const coordinate& coord) {
    colour oldCol = getColour(coord);
    colour newCol = oldCol == black ? white : black;
    setColour(coord, newCol);
}

void board::setColour(coordinate coord, colour col) {
    _boardData[coord[0]][coord[1]][coord[2]] = col;
}

reversi::reversi(int size)
        :   _board(board(size)),
            _currentPlayer(white)
    {}

void reversi::swapPlayers() {
    _currentPlayer = opponent(_currentPlayer);
}

bool reversi::play(int x, int y, int z) {
    if (x == 0 && y == 0 && z == 0) {
        swapPlayers();
        return true;
    }

    coordinate coord = _board.transform(x, y, z);

    bool valid = _board.makeMove(coord, _currentPlayer);
    if (valid) {
        swapPlayers();
    }
    return valid;
}

bool reversi::finished() const { return _board.isFull(); }

int reversi::result() const { return _board.countScore(); }

