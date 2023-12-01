#include <cassert>
#include "minmax.h"

int main() {
    assert(min(1, 2) == 1);
    assert(min(2, 1) == 1);

    assert(max(1, 2) == 2);
    assert(max(2, 1) == 2);
}
