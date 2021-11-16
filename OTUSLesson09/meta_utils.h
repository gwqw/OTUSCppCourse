#pragma once
#include <type_traits>

/// checks if type is container
template <typename T>
struct is_cont : std::false_type {};

template <typename T,typename Alloc>
struct is_cont<std::vector<T,Alloc>> : std::true_type {};

template <typename T,typename Alloc>
struct is_cont<std::list<T,Alloc>> : std::true_type {};

/// checks if all types are the same
template <typename T, typename ... Args>
struct is_all_of {};

template <typename  T, typename ... Args>
struct is_all_of<T, T, Args...> : is_all_of<T, Args...> {};

template <typename  T>
struct is_all_of<T> : std::true_type {};

template <typename T, typename  U, typename ... Args>
struct is_all_of<T, U, Args...> : std::false_type {};





