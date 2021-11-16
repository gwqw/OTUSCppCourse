#pragma once

#include <list>
#include <optional>
#include <tuple>

/**
 * @brief Sparse matrix with LIL implementation (List of lists)
 *
 * LIL stores one list per row, with each entry containing the column index and the value.
 * Typically, these entries are kept sorted by column index for faster lookup.
 * This is another format good for incremental matrix construction.
 * https://en.wikipedia.org/wiki/Sparse_matrix
 *
 * This sparse matrix implementation uses
 * std::list<{std::size_t row, std::list<{std::size_t col, T}>}> as a data storage
 * operator[][] -- O(N^2)
 */
template <typename T, T zero_value>
class SparseMatrixLIL {
private:
    struct ColNode {
        std::size_t col = 0;
        T val_ = zero_value;
        friend bool operator<(const ColNode& lhs, const ColNode& rhs) {
            return lhs.col < rhs.col;
        }
    };

    struct RowNode {
        std::size_t row = 0;
        std::list<ColNode> column_;
        friend bool operator<(const RowNode& lhs, const RowNode& rhs) {
            return lhs.row < rhs.row;
        }
    };

    using MatrixData = std::list<RowNode>;
    class ValueProxy;

    class ConstRow {
    public:
        explicit ConstRow(const SparseMatrixLIL& matrix, std::size_t row)
                : matrix_(matrix), row_(row) {}
        T operator[](std::size_t col) const {
            return matrix_.get_value_or_zero(row_, col);
        }
    private:
        const SparseMatrixLIL& matrix_;
        std::size_t row_ = 0;
    };

    class Row {
    public:
        explicit Row(SparseMatrixLIL& matrix, std::size_t row)
                : matrix_(matrix), row_(row) {}
        ValueProxy operator[](std::size_t col) const {
            return ValueProxy(matrix_, row_, col);
        }
    private:
        SparseMatrixLIL& matrix_;
        std::size_t row_ = 0;
    };

    class ValueProxy {
    public:
        ValueProxy(SparseMatrixLIL& matrix, std::size_t row, std::size_t col)
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
        SparseMatrixLIL& matrix_;
        std::size_t row_ = 0;
        std::size_t col_ = 0;
    };

    using RowIterator = typename MatrixData::iterator;
    using ColIterator = typename std::list<ColNode>::iterator;

    class Iterator {
    public:
        explicit Iterator(RowIterator rit, std::optional<ColIterator> cit, const MatrixData& data)
                : rit_(rit), data_(data) {
            if (cit) {
                cit_ = *cit;
            }
        }
        auto operator*() {
            return std::tie(rit_->row, cit_->col, cit_->val_);
        }
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return (lhs.rit_ == lhs.data_.end() && rhs.rit_ == rhs.data_.end()) ||
                   (lhs.rit_ != lhs.data_.end() && rhs.rit_ != rhs.data_.end() &&
                    std::tie(lhs.rit_, lhs.cit_) == std::tie(rhs.rit_, rhs.cit_));
        }
        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }
        Iterator& operator++() {
            ++cit_;
            if (cit_ == rit_->column_.end()) {
                ++rit_;
                if (rit_ != data_.end()) {
                    cit_ = rit_->column_.begin();
                }
            }
            return *this;
        }
        const Iterator operator++(int) {
            Iterator res = *this;
            ++(*this);
            return res;
        }
    private:
        RowIterator rit_;
        ColIterator cit_;
        const MatrixData& data_;
    };

public:
    [[nodiscard]] std::size_t size() const { return size_;}
    ConstRow operator[](std::size_t row) const { return ConstRow(*this, row); }
    Row operator[](std::size_t row) { return Row(*this, row); }
    Iterator begin() {
        if (data_.begin() != data_.end()) {
            return Iterator(data_.begin(), data_.begin()->column_.begin(), data_);
        } else {
            return Iterator(data_.begin(), std::nullopt, data_);
        }
    }
    Iterator end() {return Iterator(data_.end(), std::nullopt, data_);}
private:
    MatrixData data_;
    static constexpr T zero_value_ = zero_value;
    std::size_t size_ = 0;

    const T& get_value_or_zero(std::size_t row, std::size_t col) const {
        if (auto it = lower_bound(data_.begin(), data_.end(), RowNode{row});
                it != data_.end() && it->row == row)
        {
            const auto& column = it->column_;
            if (auto ct = lower_bound(column.begin(), column.end(), ColNode{col});
                    ct != column.end() && ct->col == col)
            {
                return ct->val_;
            }
            return zero_value_;
        } else {
            return zero_value_;
        }
    }

    void add_value(std::size_t row, std::size_t col, const T& val) {
        auto it = std::lower_bound(data_.begin(), data_.end(), RowNode{row});
        if (it == data_.end() || it->row != row) {
            it = data_.insert(it, {row});
        }
        auto& column = it->column_;
        auto ct = std::lower_bound(column.begin(), column.end(), ColNode{col});
        if (ct == column.end() || ct->col != col) {
            column.insert(ct, {col, val});
            ++size_;
        } else {
            ct->val_ = val;
        }
    }

    void delete_value(std::size_t row, std::size_t col) {
        auto it = std::lower_bound(data_.begin(), data_.end(), RowNode{row});
        if (it == data_.end() || it->row != row) {return;}
        auto& column = it->column_;
        auto ct = std::lower_bound(column.begin(), column.end(), ColNode{col});
        if (ct == column.end() || ct->col != col) {return;}
        column.erase(ct);
        if (column.empty()) {
            data_.erase(it);
        }
        --size_;
    }
};