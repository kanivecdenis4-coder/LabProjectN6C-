#pragma once
// Параметри гри та їх читання з текстового файлу.

#include <cstdint>
#include <stdexcept>
#include <string>

struct Config {
    int rows = 3;
    int cols = 3;
    int winLength = 3;
    std::uint32_t seed = 0;
    bool hasSeed = false;
};

class ConfigError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

constexpr int kMaxBoardSide = 100;

// Формат файлу: див. README.md. Кидає ConfigError при будь-якій помилці.
Config loadConfig(const std::string& path);
Config parseConfig(const std::string& text);