#include <stdio.h>

#define CLEAR_SCREEN() do { printf("\033[2J\033[H"); fflush(stdout); } while(0)


void wait_ms(int milliseconds);