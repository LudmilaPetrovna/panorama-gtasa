#include <stdint.h>

void hilbert(uint32_t i, uint32_t level, uint32_t *out_x, uint32_t *out_y);
int hilbert_points_at_level(int level);
int hilbert_side_at_level(int level);

