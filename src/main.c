#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "../include/dashboard.h"
#include "../include/utils.h"

int main(int argc, char const *argv[])
{   
    int running = 1;
    init_dashboard();

    while (running)
    {
        //wait_ms(0.2);
    }
    
    endwin();
    return 0;
}