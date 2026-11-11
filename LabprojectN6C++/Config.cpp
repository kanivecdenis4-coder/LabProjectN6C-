#include "Config.hpp"

#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>

namespace {

    long long parseInteger(const std::string& token, const char* name) {
        std::size_t pos = 0;
        long long value = 0;
        try {
            value = std::stoll(token, &pos);
        }
        catch (const std::exception&) {
            throw ConfigError(std::string("Invalid value for '") + name + "': '" + token +
                "' is not an integer");
        }
        if (pos != token.size())
            throw ConfigError(std::string("Invalid value for '") + name + "': '" + token +
                "' is not an integer");
        return value;
    }

    std::string stripComments(const std::string& text) {
        std::istringstream in(text);
        std::string line, result;
        while (std::getline(in, line)) {
            const auto hash = line.find('#');
            if (hash != std::string::npos) line.erase(hash);
            result += line;
            result += '\n';
        }
        return result;
    }

}  // namespace

Config parseConfig(const std::string& text) {
    std::istringstream in(stripComments(text));
    std::vector<std::string> tokens;
    for (std::string t; in >> t;) tokens.push_back(t);

    if (tokens.size() < 3 || tokens.size() > 4)
        throw ConfigError("Expected 3 or 4 integers: rows cols winLength [seed], found " +
            std::to_string(tokens.size()));

    Config cfg;
    const long long rows = parseInteger(tokens[0], "rows");
    const long long cols = parseInteger(tokens[1], "cols");
    const long long win = parseInteger(tokens[2], "winLength");

    if (rows < 1 || rows > kMaxBoardSide)
        throw ConfigError("rows must be in range 1.." + std::to_string(kMaxBoardSide));
    if (cols < 1 || cols > kMaxBoardSide)
        throw ConfigError("cols must be in range 1.." + std::to_string(kMaxBoardSide));
    if (win < 1 || win > std::max(rows, cols))
        throw ConfigError("winLength must be in range 1..max(rows, cols) = 1.." +
            std::to_string(std::max(rows, cols)));

    cfg.rows = static_cast<int>(rows);
    cfg.cols = static_cast<int>(cols);
    cfg.winLength = static_cast<int>(win);

    if (tokens.size() == 4) {
        const long long seed = parseInteger(tokens[3], "seed");
        if (seed < 0 || seed > static_cast<long long>(std::numeric_limits<std::uint32_t>::max()))
            throw ConfigError("seed must be in range 0..4294967295");
        cfg.seed = static_cast<std::uint32_t>(seed);
        cfg.hasSeed = true;
    }
    return cfg;
}

Config loadConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw ConfigError("Cannot open input file: " + path);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    if (file.bad()) throw ConfigError("Error while reading input file: " + path);
    return parseConfig(buffer.str());
}