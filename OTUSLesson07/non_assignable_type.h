#pragma once

#include <utility>

template <typename T>
class NonAssignable {
public:
    explicit NonAssignable(const T& value) : value_(value) {}
    NonAssignable(const NonAssignable& other) : value_(other.value_) {}
    NonAssignable(NonAssignable&& other) noexcept : value_(std::move(other.value_)) {}
    NonAssignable& operator=(const NonAssignable&) = delete;
    NonAssignable& operator=(NonAssignable&&) = delete;
    const T& get_value() const {return value_;}
    T& get_value() {return value_;}
private:
    T value_;
};