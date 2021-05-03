#include <stdint.h>
#include "includes/data.h"

void eraseArray(uint8_t * array, uint8_t size) {
   for(uint8_t i=0; i<size; i++) {
      array[i] = 0;
   }
}
