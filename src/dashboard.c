#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/utils.h"

#define LINES 8

typedef struct {
    WINDOW *win;
    int init;
} ThreadArgs;

static pthread_mutex_t screen_lock = PTHREAD_MUTEX_INITIALIZER;

void set_border(WINDOW *win, int lines){
    pthread_mutex_lock(&screen_lock);
    wattron(win, COLOR_PAIR(1));
    for(int i=0; i<=lines; i++){
        if(i==0 || i==lines)
            mvwprintw(win, i, 0, "+-----------------------------------------------------------+");
        else
            mvwprintw(win, i, 0, "|                                                           |");
    }
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
    pthread_mutex_unlock(&screen_lock);
}

void* draw_shiftLights(void* arg){
    ThreadArgs* args = arg;
    if(!args->init) return NULL;

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

    return NULL;
}

void init_colors(){
    initscr();
    noecho();
    curs_set(FALSE);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_RED, -1);
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