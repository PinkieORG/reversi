#pragma once
#include <vector>
#include <tuple>

/* The subject of this task is the game of reversi (also known as
 * othello), played by two players on a 3D board (cube) of a given
 * size (which must be an even number). The cells are cubes (a cube
 * has 8 vertices, 8 edges and 6 faces). The coordinates start at
 * the center (which is a vertex) and extend in two directions
 * (positive and negative) along the 3 axes. The 8 cells which
 * share the center vertex have coordinates [1, 1, 1], [1, 1, -1],
 * [1, -1, 1], [1, -1, -1], …
 *
 * The rules are a straightforward extension of the standard 2D
 * rules into three dimensions:
 *
 *  • each player starts with 4 stones placed around the center such
 *    that no two (of the same colour) share a face, with white
 *    taking the [1, 1, 1] cell,
 *  • players take turns in placing a new stone, which must be
 *    placed adjacent (share an edge, vertex or a face) to an
 *    opposing player's stone, and enemy stones must form a
 *    straight, uninterrupted line to one of current players'
 *    own stones (along straight lines – sharing a face, along
 *    diagonals which share an edge, or along diagonals which
 *    share a single vertex),
 *  • the colour of all opposing stones on all such lines connecting
 *    the new stone to existing stones of the current player is
 *    flipped.
 *
 * The white player starts. The game ends when no new stones can be
 * placed and the player with more stones wins. */

enum colour {none, white, black};
using coordinate = std::vector<int>;
using boardData = std::vector<std::vector<std::vector<colour>>>;

coordinate operator+ (coordinate first, coordinate second);
colour& opponent (colour& colour);
std::vector<coordinate> directions();

class board{
    boardData _boardData;
    unsigned _size;
public:
    explicit board(unsigned size);

    // Transform the game coordinates to vector indexes.
    coordinate transform(int, int, int) const;

    colour getColour(coordinate);

    void setColour(coordinate, colour);

    bool makeMove(const coordinate& coord, colour player);

    void flip(const coordinate& coord);

    bool isOnBoard(coordinate coord) const;

    bool isFull() const;

    int countScore() const;
};

class reversi {
    colour _currentPlayer;
    board _board;
public:
    explicit reversi(int size);

    void swapPlayers();

    /* Place a stone at the given coordinates. If the placement was
     * legal, returns ‹true› and the next call places a stone of the
     * opposing player; otherwise, no change is made, the function
     * returns ‹false› and the same player must try a different
     * move. The behaviour is undefined to call ‹play› when the game
     * is over. */

    bool play(int x, int y, int z);

    /* Return true if the game is finished (no further moves are
     * possible). */

    bool finished() const;

    /* Only defined if the game is already over (i.e. ‹finished›
     * would return ‹true›). Returns the difference in the number of
     * stones of each player: positive for white's victory, negative
     * for black's victory, 0 for a draw. */

    int result() const;
};


