#include <ncurses.h>
#include "../include/utils.h"

void set_border(WINDOW *win, int lines){
    int i = 0;
    while (lines >= i){
        if((i == 0) || (i == lines)){
            mvwprintw(win, i, 0, "+-----------------------------------------------------------+");
        }else{
            mvwprintw(win, i, 0, "|                                                           |");
        }

        i++;
    }
    
    wrefresh(win);
}

void init_dashboard(){
    CLEAR_SCREEN();
    initscr();
    curs_set(false);
    noecho();

    set_border(stdscr, 8);
    
    refresh();
}