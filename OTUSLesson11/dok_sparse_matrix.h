#pragma once

#include <map>
#include <tuple>
#include <utility>

/**
 * @brief Sparse matrix with DOK implementation (Dictionary of keys)
 *
 * DOK consists of a dictionary that maps (row, column)-pairs to the value of the elements.
 * Elements that are missing from the dictionary are taken to be zero.
 * The format is good for incrementally constructing a sparse matrix in random order, but poor for iterating over non-zero values in lexicographical order.
 * One typically constructs a matrix in this format and then converts to another more efficient format for processing.
 * https://en.wikipedia.org/wiki/Sparse_matrix
 *
 * This sparse matrix implementation uses
 * std::map<std::pair<std::size_t row, std::size_t col>, T value> as a data storage
 * operator[][] -- O(logN) (for std::map)
 * (if replace std::map -> std::unordered_map, complexity will be amortized O(1))
 */
template <typename T, T zero_value>
class SparseMatrixDOK {
private:
    class ValueProxy;
    using MatrixData = std::map<std::pair<std::size_t, std::size_t>, T>;
    using MapIterator = typename MatrixData::iterator;

    class ConstRow {
    public:
        explicit ConstRow(const SparseMatrixDOK& matrix, std::size_t row)
            : matrix_(matrix), row_(row) {}
        const T& operator[](std::size_t col) const {
            return matrix_.get_value_or_zero(row_, col);
        }
    private:
        const SparseMatrixDOK& matrix_;
        std::size_t row_ = 0;
    };

    class Row {
    public:
        explicit Row(SparseMatrixDOK& matrix, std::size_t row)
            : matrix_(matrix), row_(row) {}
        ValueProxy operator[](std::size_t col) const {
            return ValueProxy(matrix_, row_, col);
        }
    private:
        SparseMatrixDOK& matrix_;
        std::size_t row_ = 0;
    };

    class ValueProxy {
    public:
        ValueProxy(SparseMatrixDOK& matrix, std::size_t row, std::size_t col)
            : matrix_(matrix), row_(row), col_(col)
        {}
        
        ValueProxy& operator=(const ValueProxy& other) {
            *this = static_cast<T>(other);
            return *this;
        }

        ValueProxy& operator=(T value) {
            if (value != zero_value) {
                matrix_.add_value(row_, col_, value);
            } else {
                matrix_.delete_value(row_, col_);
            }
            return *this;
        }
        
        operator T() const {
            return std::as_const(matrix_)[row_][col_];
        }
    private:
        SparseMatrixDOK& matrix_;
        std::size_t row_ = 0;
        std::size_t col_ = 0;
    };

    class Iterator {
    public:
        explicit Iterator(MapIterator it) : it_(it) {}
        auto operator*() {
            const auto& idx_pair = it_->first;
            return std::tie(idx_pair.first, idx_pair.second, it_->second);
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

public:
    [[nodiscard]] std::size_t size() const {return data_.size();}
    ConstRow operator[](std::size_t row) const {return ConstRow(*this, row);}
    Row operator[](std::size_t row) {return Row(*this, row);}
    Iterator begin() {return Iterator(data_.begin());}
    Iterator end() {return Iterator(data_.end());}
private:
    MatrixData data_;
    static constexpr T zero_value_ = zero_value;

    const T& get_value_or_zero(std::size_t row, std::size_t col) const {
        if (auto it = data_.find({row, col}); it != data_.end()) {
            return it->second;
        }
        return zero_value_;
    }

    void add_value(std::size_t row, std::size_t col, const T& val) {
        data_[{row, col}] = val;
    }

    void delete_value(std::size_t row, std::size_t col) {
        data_.erase({row, col});
    }
};

