#pragma once
// Керування партією: по черзі відновлює сопрограми гравців і веде протокол.

#include <cstdint>
#include <iosfwd>

#include "Board.hpp"
#include "Config.hpp"

enum class GameResult { XWins, OWins, Draw };

class Game {
public:
    explicit Game(const Config& config, std::uint32_t seed);

    // Грає партію до кінця, пише протокол у out, повертає результат.
    GameResult run(std::ostream& out);

private:
    Board board_;
    std::uint32_t seed_;
};