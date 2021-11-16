#pragma once

struct hard {
    int fa;
    int fi;
    hard(int fa, int fi) : fa(fa), fi(fi) {};
    hard(const hard&) = delete;
    hard(hard&&) = delete;
};