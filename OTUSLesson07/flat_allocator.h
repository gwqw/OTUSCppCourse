#pragma once

#include <iostream>
#include <utility>

template <typename T, std::size_t N>
class FlatAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    FlatAllocator() = default;

    FlatAllocator(const FlatAllocator& other) = delete;

    FlatAllocator(FlatAllocator&& other) noexcept
            : head_(other.head_), cp_(other.cp_) {
        other.head_ = nullptr;
    }
    
    ~FlatAllocator() {
        if (head_) {
            free(head_);
        }
    }

    FlatAllocator& operator=(const FlatAllocator& other) = delete;
    
    template<typename U>
    struct rebind {
        using other = FlatAllocator<U, N>;
    };
    
    T* allocate(std::size_t n) {
        if (!cp_) {
            static_assert(N < std::numeric_limits<std::size_t>::max() / sizeof(T));
            head_ = static_cast<T*>(std::malloc(N*sizeof(T)));
            if (!head_) {
                throw std::bad_alloc();
            }
            cp_ = head_;
        }

        if ((cp_ - head_) + n <= N) {
            auto res = cp_;
            cp_ += n;
            return res;
        }

        throw std::bad_alloc();
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
    T* head_ = nullptr;
    T* cp_ = nullptr;
};

template< class T1, class T2, int N1, int N2>
bool operator==( const FlatAllocator<T1, N1>& lhs, const FlatAllocator<T2, N2>& rhs ) noexcept {
    return (N1 == N2);
}

template< class T1, class T2, int N1, int N2>
bool operator!=( const FlatAllocator<T1, N1>& lhs, const FlatAllocator<T2, N2>& rhs ) noexcept {
    return !(lhs == rhs);
}

