#pragma once

#include <iostream>

template <typename T>
struct LogAllocator {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    
    template<typename U>
    struct rebind {
        using other = LogAllocator<U>;
    };
    
    T* allocate(std::size_t n) const {
        std::cout << __PRETTY_FUNCTION__ << " n= " << n << std::endl;
        if(n > std::numeric_limits<std::size_t>::max() / sizeof(T)) throw std::bad_alloc();
        if(auto p = static_cast<T*>(std::malloc(n*sizeof(T)))) return p;
        throw std::bad_alloc();
    }
    
    void deallocate(T* p, std::size_t n) const {
        std::cout << __PRETTY_FUNCTION__ << " n= " << n << std::endl;
        free(p);
    }
    
    template <typename U, typename ... Args>
    void construct(U* p, Args&& ... args) const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        new (p) U(std::forward<Args>(args)...);
    }
    
    template <typename U>
    void destroy(U* p) const {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        p->~U();
    }
};

template< class T1, class T2 >
bool operator==( const LogAllocator<T1>& lhs, const LogAllocator<T2>& rhs ) noexcept {
    return true;
}

template< class T1, class T2 >
bool operator!=( const LogAllocator<T1>& lhs, const LogAllocator<T2>& rhs ) noexcept {
    return false;
}
