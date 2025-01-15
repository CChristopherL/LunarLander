#ifndef MATH_H_
#define MATH_H_

#include <stdint.h>

int16_t sinLUT(int16_t x);
int16_t cosLUT(int16_t x);

typedef struct {int32_t x, y;} vector_t;

void rotateVector(vector_t *v, int16_t theta);

#endif /* MATH_H_ */
