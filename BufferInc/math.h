#ifndef MATH_H_
#define MATH_H_

#include <stdint.h>

typedef struct {int32_t x; int32_t y;} vector_t;

int16_t sinLUT(int16_t x);
int16_t cosLUT(int16_t x);

void rotateVector(vector_t *v, int16_t theta);

#endif /* MATH_H_ */
