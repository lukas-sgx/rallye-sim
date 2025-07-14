#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include "../include/utils.h"

#define LINES 8

void set_border(WINDOW *win, int lines){
    int i = 0;
    wattron(win,COLOR_PAIR(1));   
    while (lines >= i){
        if((i == 0) || (i == lines)){
            mvwprintw(win, i, 0, "+-----------------------------------------------------------+");
        }else{
            mvwprintw(win, i, 0, "|                                                           |");
        }

        i++;
    }
    wattroff(win, COLOR_PAIR(1));
    
    wrefresh(win);
}

void draw_shiftLigths(WINDOW *win, int init){
    if(init){
        int i = 0;
        int colorIter = 2;
        char buffer[40] = "";

        mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
        wrefresh(win);
        wait_ms(1000);

        while(colorIter <= 4){
            wattron(win, COLOR_PAIR(1)); 
            mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
            wrefresh(win);
            buffer[0] = '\0';
            wattroff(win, COLOR_PAIR(1)); 

            wattron(win, COLOR_PAIR(colorIter)); 

            while (i <= 9){
                size_t len = strlen(buffer);

                if (i == 0){
                    if (len < sizeof(buffer) - 4) { 
                        snprintf(buffer + len, sizeof(buffer) - len, "●");
                    }
                }else{
                    if (len < sizeof(buffer) - 4) { 
                        snprintf(buffer + len, sizeof(buffer) - len, " ●");
                    }
                }
                mvwprintw(win, 1, 21, buffer);
                
                wait_ms(100);
                wrefresh(win);
                i++;
                
            }

            wattroff(win, COLOR_PAIR(colorIter));  
            colorIter++;

            if (i == 10){
                i = 0;
            }
            wait_ms(200);
        }
        
        wait_ms(100);
        if (colorIter == 5){
            int a = 0;

            while (a <= 2)
            {
                wattron(win, COLOR_PAIR(1)); 
                mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
                wrefresh(win);
                wait_ms(200);
                wattroff(win, COLOR_PAIR(1)); 
                wattron(win, COLOR_PAIR(4)); 
                mvwprintw(win, 1, 21, "● ● ● ● ● ● ● ● ● ●");
                wrefresh(win);
                wait_ms(200);
                wattroff(win, COLOR_PAIR(4)); 
                a++;
            }

            wattron(win, COLOR_PAIR(1)); 
            mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
            wrefresh(win);
            wattroff(win, COLOR_PAIR(1)); 
        }       
    }
    
    wrefresh(win);
}

void draw_rpm_bargraph(int rpm){

}

void draw_info_panel(){

}

void init_colors() {
    initscr();
    noecho();
    curs_set(FALSE);
    start_color();      
    use_default_colors();   

    init_pair(1, COLOR_WHITE, -1);   // Texte normal
    init_pair(2, COLOR_GREEN, -1);  // Shift lights warning
    init_pair(3, COLOR_YELLOW, -1);     // RPM / shift lights rouge 
    init_pair(4, COLOR_RED, -1);   // Bargraph
    init_pair(5, COLOR_BLUE, -1);    // Eau
    init_pair(6, COLOR_CYAN, -1);    // Boost
    init_pair(7, COLOR_MAGENTA, -1); // Huile
}

void init_dashboard(){
    CLEAR_SCREEN();
    setlocale(LC_ALL, "");
    init_colors();

    set_border(stdscr, LINES);
    draw_shiftLigths(stdscr, 1);
    
    refresh();
}