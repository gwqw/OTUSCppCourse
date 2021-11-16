#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <array>

template <typename T, std::size_t N>
class ChunkAllocator {
private:
    struct ChunkNode{
        ChunkNode* next = nullptr;
        T* data = nullptr;
    };
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    ChunkAllocator() {
        static_assert(N);
        static_assert(N < std::numeric_limits<std::size_t>::max() / sizeof(T));
    }

    ChunkAllocator(const ChunkAllocator& other) = delete;

    ChunkAllocator(ChunkAllocator&& other) noexcept
            : head_(other.head_), cur_data_(other.cur_data_), idx_(other.idx_), cp_(other.cp_) {
        other.head_ = nullptr;
    }

    ~ChunkAllocator() {
        while (head_) {
            auto p = head_;
            head_ = head_->next;
            if (p->data) {
                free(p->data);
            }
            free(p);
        }
    }
    
    template<typename U>
    struct rebind {
        using other = ChunkAllocator<U, N>;
    };
    
    T* allocate(std::size_t n) {
        if (n > N) {
            throw std::bad_alloc();
        }
        if ((idx_ + n) > N) {idx_ = 0;}
        if (idx_ == 0) {
            cp_ = allocate_new_chunk();
        }

        auto res = cp_;
        idx_ += n;
        cp_ += n;
        return res;
    }
    
    void deallocate(T* p, std::size_t n) const {
        // do nothing
    }
    
    template <typename U, typename ... Args>
    void construct(U* p, Args&& ... args) const {
        new (p) U(std::forward<Args>(args)...);
    }
    
    template <typename U>
    void destroy(U* p) const {
        p->~U();
    }
    
private:
    //std::vector<T*> data_;
    ChunkNode* head_ = nullptr;
    ChunkNode* cur_data_ = nullptr;
    std::size_t idx_ = 0;
    T* cp_ = nullptr;
    // methods
    T* allocate_new_chunk() {
        auto n = static_cast<ChunkNode*>(std::malloc(sizeof(ChunkNode)));
        if (!n) throw std::bad_alloc();
        n->data = static_cast<T*>(std::malloc(N*sizeof(T)));
        n->next = nullptr;
        if (!n->data) throw std::bad_alloc();
        if (cur_data_) {
            cur_data_->next = n;
        } else {
            head_ = n;
        }
        cur_data_ = n;
        return n->data;
    }
};

template< class T1, class T2, int N1, int N2>
bool operator==( const ChunkAllocator<T1, N1>& lhs, const ChunkAllocator<T2, N2>& rhs ) noexcept {
    return (N1 == N2);
}

template< class T1, class T2, int N1, int N2>
bool operator!=( const ChunkAllocator<T1, N1>& lhs, const ChunkAllocator<T2, N2>& rhs ) noexcept {
    return !(lhs == rhs);
}
