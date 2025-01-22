#ifndef FUEL_GAUGE_H_
#define FUEL_GAUGE_H_

#include <stdint.h>

#define NONE 0
#define GREEN 1
#define YELLOW 2
#define RED 3

void updateFuelGauge(uint32_t *runtime, int16_t fuel);

#endif /* FUEL_GAUGE_H_ */
