#include "util.h"

f32 randf() {
    return (float)rand() / (float)RAND_MAX;
}

f32 randf_range(f32 r_min, f32 r_max) {
    return randf() * (r_max - r_min) + r_min;
}