#include "Player.hpp"

#include <random>

Generator<Move> playerCoroutine(const Board& board, std::uint32_t seed) {
    std::mt19937 rng(seed);
    while (true) {
        const auto free = board.freeCells();
        if (free.empty()) co_return;  // ходити нікуди
        std::uniform_int_distribution<std::size_t> pick(0, free.size() - 1);
        co_yield free[pick(rng)];
    }
}