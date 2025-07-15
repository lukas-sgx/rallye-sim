#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "../include/dashboard.h"
#include "../include/utils.h"

int main(int argc, char const *argv[])
{   
    init_dashboard();

    endwin();
    return 0;
}