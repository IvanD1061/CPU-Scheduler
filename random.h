#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
/**
 * Reads a random intger from the givenfile named random-numbers.
 * Returns the CPU Burst
 */
extern uint32_t randomOS(uint32_t upper_bound, uint32_t process_indx, FILE *random_num_file_ptr);