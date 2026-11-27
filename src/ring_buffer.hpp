#pragma once

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace telemetry {

/// Bounded FIFO circular buffer with a fixed capacity chosen at construction.
/// Not thread-safe. Storage is owned via std::vector (RAII); T need not be
/// default-constructible.
template <typename T>
class RingBuffer {
public:
    /// @throws std::invalid_argument if capacity == 0.
    explicit RingBuffer(std::size_t capacity) : slots_(validate(capacity)) {}

    /// Appends a value. Returns false (and leaves the buffer unchanged) if full.
    [[nodiscard]] bool push(T value) {
        if (full()) {
            return false;
        }
        slots_[tail_].emplace(std::move(value));
        tail_ = next(tail_);
        ++size_;
        return true;
    }

    /// Removes and returns the oldest value, or std::nullopt if empty.
    [[nodiscard]] auto pop() -> std::optional<T> {
        if (empty()) {
            return std::nullopt;
        }
        std::optional<T> out{std::move(*slots_[head_])};
        slots_[head_].reset();
        head_ = next(head_);
        --size_;
        return out;
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] std::size_t capacity() const noexcept { return slots_.size(); }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] bool full() const noexcept { return size_ == slots_.size(); }

private:
    static std::size_t validate(std::size_t capacity) {
        if (capacity == 0) {
            throw std::invalid_argument("RingBuffer capacity must be greater than 0");
        }
        return capacity;
    }

    [[nodiscard]] std::size_t next(std::size_t index) const noexcept {
        return (index + 1) % slots_.size();
    }

    std::vector<std::optional<T>> slots_;
    std::size_t head_{0};
    std::size_t tail_{0};
    std::size_t size_{0};
};

}  // namespace telemetry
