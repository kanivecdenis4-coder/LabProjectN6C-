#include "Game.hpp"

#include <array>
#include <ostream>
#include <stdexcept>

#include "Player.hpp"

Game::Game(const Config& config, std::uint32_t seed)
    : board_(config.rows, config.cols, config.winLength), seed_(seed) {
}

GameResult Game::run(std::ostream& out) {
    // Різні початкові значення генераторів, щоб гравці грали по-різному.
    std::array<Generator<Move>, 2> players{ playerCoroutine(board_, seed_),
                                           playerCoroutine(board_, seed_ ^ 0x9E3779B9u) };
    const std::array<Mark, 2> marks{ Mark::X, Mark::O };

    out << "Game: " << board_.rows() << "x" << board_.cols() << ", " << board_.winLength()
        << " in a row to win, seed=" << seed_ << "\n\n";

    int moveNumber = 0;
    std::size_t current = 0;
    while (!board_.isFull()) {
        const auto move = players[current].next();
        if (!move) throw std::logic_error("Player coroutine finished unexpectedly");

        board_.place(*move, marks[current]);
        ++moveNumber;

        out << "Move " << moveNumber << ": player " << static_cast<char>(marks[current])
            << " -> (row " << move->row + 1 << ", col " << move->col + 1 << ")\n"
            << board_ << '\n';

        if (board_.isWinningMove(*move)) {
            out << "Player " << static_cast<char>(marks[current]) << " wins!\n";
            return current == 0 ? GameResult::XWins : GameResult::OWins;
        }
        current = 1 - current;
    }
    out << "The game is finished: draw.\n";
    return GameResult::Draw;
}