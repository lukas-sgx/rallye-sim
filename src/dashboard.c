#include <ncurses.h>
#include <locale.h>
#include "../include/utils.h"

#define LINES 8

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

void draw_shiftLigths(WINDOW *win){
    mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
    wrefresh(win);
}

void draw_rpm_bargraph(int rpm){

}

void draw_info_panel(){

}

void init_dashboard(){
    CLEAR_SCREEN();
    setlocale(LC_ALL, "");
    initscr();
    curs_set(false);
    noecho();

    set_border(stdscr, LINES);
    draw_shiftLigths(stdscr);
    
    refresh();
}