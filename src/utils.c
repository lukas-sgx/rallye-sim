#include <unistd.h>

void wait_ms(int ms){
    usleep(ms * 1000);
}