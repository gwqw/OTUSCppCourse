#include <iostream>
#include <map>
#include <forward_list>

#include "log_allocator.h"
#include "flat_allocator.h"
#include "chunk_allocator.h"
#include "hard.h"
#include "simple_math.h"
#include "simple_list.h"

using namespace std;

std::ostream& operator<<(std::ostream& out, const hard& h) {
    out << h.fa << ' ' << h.fi;
    return out;
}

int main() {
    const size_t N = 10;
    // std::allocator for map
    {
        map<int, hard> first_map;
        for (int i = 0; i < (int)N; ++i) {
            first_map.try_emplace(i, factorial(i), fibonacci(i));
        }
    }

    // new_allocator for map
    {
        map<int, hard, std::less<>, FlatAllocator<pair<const int, hard>, N>> second_map;
        for (int i = 0; i < (int)N; ++i) {
            second_map.try_emplace(i, factorial(i), fibonacci(i));
        }
        // outputs second map
        for (const auto& [i, h] : second_map) {
            cout << i << ' ' << h << '\n';
        }
    }

    // new container
    {
        MyList<hard> lst;
        for (int i = int(N)-1; i >= 0; --i) {
            lst.emplace_front(factorial(i), fibonacci(i));
        }
        //cout << lst << endl;
    }

    // new container + new allocator
    {
        MyList<hard, FlatAllocator<hard, N>> lst;
        for (int i = int(N)-1; i >= 0; --i) {
            lst.emplace_front(factorial(i), fibonacci(i));
        }
        cout << lst << endl;
    }

    // new container + new allocator that can enlarge
    {
        map<int, hard, std::less<>, ChunkAllocator<pair<const int, hard>, N>> second_map;
        for (int i = 0; i < (int)N+1; ++i) {
            second_map.try_emplace(i, factorial(i), fibonacci(i));
        }
        // outputs second map
        for (const auto& [i, h] : second_map) {
            cout << i << ' ' << h << '\n';
        }
    }

    return 0;
}