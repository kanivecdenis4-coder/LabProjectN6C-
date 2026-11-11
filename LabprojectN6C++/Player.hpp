#pragma once
// Гравець реалізований як сопрограма: при кожному відновленні вона дивиться на
// поточний стан поля та віддає (co_yield) наступний хід.

#include <cstdint>

#include "Board.hpp"
#include "Generator.hpp"

// Випадкова (не оптимальна) стратегія: обирає довільну вільну клітинку.
// board має жити довше за сопрограму.
Generator<Move> playerCoroutine(const Board& board, std::uint32_t seed);