#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "../include/data.h"
#include "../include/utils.h"

#define LINES 8

typedef struct {
    WINDOW *win;
    int init;
} ThreadArgs;

static pthread_mutex_t screen_lock = PTHREAD_MUTEX_INITIALIZER;

void set_border(WINDOW *win, int lines){
    wattron(win, COLOR_PAIR(1));
    for(int i = 0; i <= lines; i++){
        if(i == 0 || i == lines)
            mvwprintw(win, i, 0, "+-----------------------------------------------------------+");
        else
            mvwprintw(win, i, 0, "|                                                           |");
    }
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
}



void draw_rpm_bargraph(int rpm){

}

void draw_info_panel(WINDOW *win, float rpm, float throttle, int gear, int speed){
    float boostParam = -0.4f;
    float T_water = 25.0f;
    float T_amb = 25.0f;

    wattron(win, COLOR_PAIR(6));
    boostParam = get_boost(rpm, throttle, boostParam, 1.5f, 0.05f);
    mvwprintw(win, 3, 4, "BOOST  %.1f", boostParam);
    wrefresh(win);
    wattroff(win, COLOR_PAIR(6));

    if(gear == 0){
        mvwprintw(win, 3, 26, "[   N   ]");
    }else if(gear == -1){
        mvwprintw(win, 3, 26, "[   R   ]");
    }else{
        mvwprintw(win, 3, 26, "[   %u   ]", gear);
    }
    wrefresh(win);

    wattron(win, COLOR_PAIR(5));
    T_water = get_WaterTemp(T_water, rpm, throttle, T_amb, 100);
    if (T_water < 10 && T_water > -10){
        mvwprintw(win, 3, 61-17, "WATER T°  %.0f°C", T_water);
    }else{
        mvwprintw(win, 3, 61-17, "WATER T° %.0f°C", T_water);
    }
    wrefresh(win);
    wattroff(win, COLOR_PAIR(5));

    wait_ms(100);
}

void* draw_shiftLights(void* arg){
    ThreadArgs* args = arg;
    if(!args->init){

    }else{
        char buffer[128];
        int colorIter = 2;

        pthread_mutex_lock(&screen_lock);
        mvwprintw(args->win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
        wrefresh(args->win);
        pthread_mutex_unlock(&screen_lock);
        wait_ms(1000);

        while(colorIter <= 4){
            pthread_mutex_lock(&screen_lock);
            wattron(args->win, COLOR_PAIR(1));
            mvwprintw(args->win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
            wattroff(args->win, COLOR_PAIR(1));
            pthread_mutex_unlock(&screen_lock);

            buffer[0]='\0';
            size_t max = sizeof(buffer)-12;

            pthread_mutex_lock(&screen_lock);
            wattron(args->win, COLOR_PAIR(colorIter));

            for(int i = 0; i < 10; i++){
                if(strlen(buffer)+4 < max){
                    strcat(buffer, i? " ●":"●");
                }
                mvwprintw(args->win, 1, 21, "%-s", buffer);
                wrefresh(args->win);
                wait_ms(100);
            }
            wattroff(args->win, COLOR_PAIR(colorIter));
            pthread_mutex_unlock(&screen_lock);

            colorIter++;
            wait_ms(200);
        }

        pthread_mutex_lock(&screen_lock);
        for(int a = 0; a < 3; a++){
            mvwprintw(args->win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
            wrefresh(args->win);
            wait_ms(200);

            wattron(args->win, COLOR_PAIR(4));
            mvwprintw(args->win, 1, 21, "● ● ● ● ● ● ● ● ● ●");
            wrefresh(args->win);
            wattroff(args->win, COLOR_PAIR(4));
            wait_ms(200);
        }
        mvwprintw(args->win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
        wrefresh(args->win);
        pthread_mutex_unlock(&screen_lock);

        draw_info_panel(args->win, 800, 0, 0, 0);

        return NULL;
    }
}

void init_colors(){
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

void dashboard_loop(WINDOW *win){
    pthread_t thread1;
    ThreadArgs *args = malloc(sizeof *args);
    args->win = win;
    args->init = 1;

    int ch, started = 0;
    while((ch=getch())!= 'q'){
        if(ch=='s' && !started){
            pthread_create(&thread1, NULL, draw_shiftLights, args);
            started = 1;
        }
        wait_ms(20);
    }

    if(started) pthread_join(thread1, NULL);
    free(args);
}

void init_dashboard(){
    setlocale(LC_ALL, "");
    CLEAR_SCREEN();
    init_colors();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    set_border(stdscr, LINES);
    dashboard_loop(stdscr);
}