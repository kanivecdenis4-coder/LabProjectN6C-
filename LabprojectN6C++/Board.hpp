#pragma once
// Ігрове поле: зберігання клітинок, перевірка ходів та умов перемоги.

#include <cstddef>
#include <iosfwd>
#include <vector>

enum class Mark : char { Empty = '.', X = 'X', O = 'O' };

struct Move {
    int row = 0;  // 0-based
    int col = 0;  // 0-based
};

class Board {
public:
    // rows x cols - розміри поля, winLength - скільки поспіль потрібно для перемоги.
    Board(int rows, int cols, int winLength);

    int rows() const { return rows_; }
    int cols() const { return cols_; }
    int winLength() const { return winLength_; }

    Mark at(int row, int col) const;
    bool inside(int row, int col) const;
    bool isFree(int row, int col) const;
    bool isFull() const { return freeCount_ == 0; }

    // Ставить позначку; кидає std::invalid_argument при некоректному ході.
    void place(const Move& move, Mark mark);

    // Чи утворює остання позначка в клітинці (row, col) виграшну лінію.
    bool isWinningMove(const Move& move) const;

    std::vector<Move> freeCells() const;

    friend std::ostream& operator<<(std::ostream& os, const Board& board);

private:
    std::size_t index(int row, int col) const {
        return static_cast<std::size_t>(row) * static_cast<std::size_t>(cols_) +
            static_cast<std::size_t>(col);
    }
    int countDirection(int row, int col, int dr, int dc, Mark mark) const;

    int rows_;
    int cols_;
    int winLength_;
    std::size_t freeCount_;
    std::vector<Mark> cells_;
};