#ifndef FUEL_GAUGE_H_
#define FUEL_GAUGE_H_

#define NONE 0
#define GREEN 1
#define YELLOW 2
#define RED 3

#include <stdint.h>

void updateFuelGauge(uint32_t *runtime, uint16_t fuel);

#endif /* FUEL_GAUGE_H_ */
