// Лабораторна робота, варіант 14: "Хрестики-нолики" на сопрограмах C++20.
// Використання: tictactoe [input.txt]
// Без аргументів грає на полі 3x3 (3 поспіль) з випадковим seed.

#include <cstdint>
#include <exception>
#include <iostream>
#include <random>

#include "Config.hpp"
#include "Game.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc > 2) {
            std::cerr << "Usage: " << argv[0] << " [input.txt]\n";
            return 1;
        }
        Config config;
        if (argc == 2) config = loadConfig(argv[1]);

        const std::uint32_t seed = config.hasSeed ? config.seed : std::random_device{}();

        Game game(config, seed);
        game.run(std::cout);
        return 0;
    }
    catch (const ConfigError& e) {
        std::cerr << "Input error: " << e.what() << '\n';
        return 2;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 3;
    }
}