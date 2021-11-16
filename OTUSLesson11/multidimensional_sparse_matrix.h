#pragma once

#include <map>
#include <tuple>
#include <utility>
#include <array>

template <typename T, T zero_value, std::size_t N>
class MultiSparseMatrixDOK {
private:
    static_assert(N != 0);
    class ValueProxy;
    using MatrixData = std::map<std::array<size_t, N>, T>;
    using MapIterator = typename MatrixData::iterator;
    using Indexes = std::array<size_t, N>;

    class ValueProxy {
    public:
        ValueProxy(MultiSparseMatrixDOK& matrix, Indexes indexes)
                : matrix_(matrix), indexes_(std::move(indexes))
        {}

        ValueProxy& operator=(const ValueProxy& other) {
            *this = static_cast<T>(other);
            return *this;
        }

        ValueProxy& operator=(T value) {
            if (value != zero_value) {
                matrix_.add_value(indexes_, value);
            } else {
                matrix_.delete_value(indexes_);
            }
            return *this;
        }

        operator T() const {
            return std::as_const(matrix_).get_value_or_zero(indexes_);
        }
    private:
        MultiSparseMatrixDOK& matrix_;
        Indexes indexes_;
    };

    class Iterator {
    public:
        explicit Iterator(MapIterator it) : it_(it) {}
        auto operator*() {
            return std::tuple_cat(it_->first, std::make_tuple(it_->second));
            //return *it_;
        }
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.it_ == rhs.it_;
        }
        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return lhs.it_ != rhs.it_;
        }
        Iterator& operator++() {
            ++it_;
            return *this;
        }
        const Iterator operator++(int) {
            Iterator res = *this;
            ++it_;
            return res;
        }
    private:
        MapIterator it_;
    };

    template <std::size_t M>
    class IndexHolder {
    public:
        explicit IndexHolder(MultiSparseMatrixDOK& matrix,
                const Indexes& indexes)
            : indexes_{indexes}, matrix_(matrix)
        {}
        auto operator[](std::size_t idx) {
            indexes_[M] = idx;
            if constexpr (M+1 != N) {
                return IndexHolder<M+1u>(matrix_, indexes_);
            } else {
                return ValueProxy(matrix_, indexes_);
            }
        }
    private:
        Indexes indexes_;
        MultiSparseMatrixDOK& matrix_;
    };

    template <std::size_t M>
    class ConstIndexHolder {
    public:
        explicit ConstIndexHolder(const MultiSparseMatrixDOK& matrix,
                             const Indexes& indexes)
                : indexes_{indexes}, matrix_(matrix)
        {}
        auto operator[](std::size_t idx) {
            indexes_[M] = idx;
            if constexpr (M+1 != N) {
                return ConstIndexHolder<M+1u>(matrix_, indexes_);
            } else {
                return matrix_.get_value_or_zero(indexes_);
            }
        }
    private:
        Indexes indexes_;
        const MultiSparseMatrixDOK& matrix_;
    };

public:
    [[nodiscard]] std::size_t size() const {return data_.size();}

    template <typename ... IdxType>
    const T& operator()(IdxType ... indexes) const {
        static_assert(sizeof...(indexes) == N);
        return get_value_or_zero(Indexes{static_cast<std::size_t>(indexes)...});
    }

    template <typename ... IdxType>
    ValueProxy operator()(IdxType ... indexes) {
        static_assert(sizeof...(indexes) == N);
        return ValueProxy(*this, Indexes{static_cast<std::size_t>(indexes)...});
    }
    ConstIndexHolder<1u> operator[](std::size_t row) const {
        Indexes tmp;
        std::fill(tmp.begin(), tmp.end(), 0u);
        tmp[0] = row;
        return ConstIndexHolder<1u>(*this, tmp);
    }
    IndexHolder<1u> operator[](std::size_t row) {
        Indexes tmp;
        std::fill(tmp.begin(), tmp.end(), 0u);
        tmp[0] = row;
        return IndexHolder<1u>(*this, tmp);
    }
    Iterator begin() {return Iterator(data_.begin());}
    Iterator end() {return Iterator(data_.end());}
private:
    MatrixData data_;
    static constexpr T zero_value_ = zero_value;

    const T& get_value_or_zero(const Indexes& indexes) const {
        if (auto it = data_.find(indexes); it != data_.end()) {
            return it->second;
        }
        return zero_value_;
    }

    void add_value(const Indexes& indexes, const T& val) {
        data_[indexes] = val;
    }

    void delete_value(const Indexes& indexes) {
        data_.erase(indexes);
    }
};

