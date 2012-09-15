#pragma once

#include <Math/RandomNumber.h>

template<class Foo>
void shuffle(Foo items[], int count, int seed=235209)
{
    RandomNumber r(seed);
    for(int i=0; i<count; i++){
        int j = (r++)(0, count);
        Foo x = items[i];
        items[i] = items[j];
        items[j] = x;
    }
}
