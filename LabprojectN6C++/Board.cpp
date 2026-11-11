#include "Board.hpp"

#include <ostream>
#include <stdexcept>

Board::Board(int rows, int cols, int winLength)
    : rows_(rows),
    cols_(cols),
    winLength_(winLength),
    freeCount_(static_cast<std::size_t>(rows)* static_cast<std::size_t>(cols)),
    cells_(freeCount_, Mark::Empty) {
    if (rows <= 0 || cols <= 0 || winLength <= 0)
        throw std::invalid_argument("Board: dimensions and win length must be positive");
}

bool Board::inside(int row, int col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
}

Mark Board::at(int row, int col) const {
    if (!inside(row, col)) throw std::out_of_range("Board::at: cell is outside the board");
    return cells_[index(row, col)];
}

bool Board::isFree(int row, int col) const {
    return inside(row, col) && cells_[index(row, col)] == Mark::Empty;
}

void Board::place(const Move& move, Mark mark) {
    if (mark == Mark::Empty) throw std::invalid_argument("Board::place: empty mark");
    if (!isFree(move.row, move.col))
        throw std::invalid_argument("Board::place: cell is occupied or outside the board");
    cells_[index(move.row, move.col)] = mark;
    --freeCount_;
}

int Board::countDirection(int row, int col, int dr, int dc, Mark mark) const {
    int count = 0;
    row += dr;
    col += dc;
    while (inside(row, col) && cells_[index(row, col)] == mark) {
        ++count;
        row += dr;
        col += dc;
    }
    return count;
}

bool Board::isWinningMove(const Move& move) const {
    const Mark mark = at(move.row, move.col);
    if (mark == Mark::Empty) return false;
    static const int dirs[4][2] = { { 0, 1 },{ 1, 0 },{ 1, 1 },{ 1, -1 } };
    for (const auto& d : dirs) {
        const int total = 1 + countDirection(move.row, move.col, d[0], d[1], mark) +
            countDirection(move.row, move.col, -d[0], -d[1], mark);
        if (total >= winLength_) return true;
    }
    return false;
}

std::vector<Move> Board::freeCells() const {
    std::vector<Move> result;
    result.reserve(freeCount_);
    for (int r = 0; r < rows_; ++r)
        for (int c = 0; c < cols_; ++c)
            if (cells_[index(r, c)] == Mark::Empty) result.push_back({ r, c });
    return result;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    for (int r = 0; r < board.rows_; ++r) {
        for (int c = 0; c < board.cols_; ++c) {
            if (c > 0) os << ' ';
            os << static_cast<char>(board.cells_[board.index(r, c)]);
        }
        os << '\n';
    }
    return os;
}