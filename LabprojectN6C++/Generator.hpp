#pragma once
// Мінімальний генератор на основі C++20 сопрограм: co_yield повертає значення,
// next() відновлює виконання сопрограми до наступного co_yield.

#include <coroutine>
#include <exception>
#include <optional>
#include <utility>

template <typename T>
class Generator {
public:
    struct promise_type;
    using Handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        T value{};
        std::exception_ptr exception;

        Generator get_return_object() { return Generator{ Handle::from_promise(*this) }; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            value = std::move(v);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { exception = std::current_exception(); }
    };

    explicit Generator(Handle h) : handle_(h) {}
    Generator(Generator&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            destroy();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    ~Generator() { destroy(); }

    // Наступне значення або std::nullopt, якщо сопрограма завершилась.
    std::optional<T> next() {
        if (!handle_ || handle_.done()) return std::nullopt;
        handle_.resume();
        if (handle_.promise().exception) std::rethrow_exception(handle_.promise().exception);
        if (handle_.done()) return std::nullopt;
        return handle_.promise().value;
    }

private:
    void destroy() {
        if (handle_) {
            handle_.destroy();
            handle_ = {};
        }
    }
    Handle handle_{};
};