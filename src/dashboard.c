#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#include "../include/data.h"
#include "../include/utils.h"

#define LINES 7
#define MAX_GEAR 6
#define MIN_GEAR -1

#define RPM_FACTOR 40.0f

static const float gear_ratio[6] = {
    3.5f, 2.2f, 1.5f, 1.2f, 1.0f, 0.85f
};

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

void draw_shiftLights(WINDOW *win, int init, float rpm){
    if(!init){
        return;
    } else {
        char buffer[128];
        int colorIter = 2;

        mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
        wrefresh(win);
        wait_ms(1000);

        while(colorIter <= 4){
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
            wattroff(win, COLOR_PAIR(1));

            buffer[0]='\0';
            size_t max = sizeof(buffer)-12;

            wattron(win, COLOR_PAIR(colorIter));

            for(int i = 0; i < 10; i++){
                if(strlen(buffer)+4 < max){
                    strcat(buffer, i? " ●":"●");
                }
                mvwprintw(win, 1, 21, "%-s", buffer);
                wrefresh(win);
                wait_ms(100);
            }
            wattroff(win, COLOR_PAIR(colorIter));

            colorIter++;
            wait_ms(200);
        }

        for(int a = 0; a < 3; a++){
            mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
            wrefresh(win);
            wait_ms(200);

            wattron(win, COLOR_PAIR(4));
            mvwprintw(win, 1, 21, "● ● ● ● ● ● ● ● ● ●");
            wrefresh(win);
            wattroff(win, COLOR_PAIR(4));
            wait_ms(200);
        }
        mvwprintw(win, 1, 21, "○ ○ ○ ○ ○ ○ ○ ○ ○ ○");
        wrefresh(win);
    }
}

void draw_info_panel(WINDOW *win, float rpm, float throttle, int gear, int speed, float boostParam, float T_water, float oilPressure){
    wattron(win, COLOR_PAIR(6));
    mvwprintw(win, 3, 4, "BOOST  %.1f", boostParam);
    wattroff(win, COLOR_PAIR(6));

    if(gear == 0){
        mvwprintw(win, 3, 26, "[   N   ]");
    } else if(gear == -1){
        mvwprintw(win, 3, 26, "[   R   ]");
    } else {
        mvwprintw(win, 3, 26, "[   %d   ]", gear);
    }

    wattron(win, COLOR_PAIR(5));
    mvwprintw(win, 3, 61-17, "WATER T° %3.0f°C", T_water);
    wattroff(win, COLOR_PAIR(5));

    wattron(win, COLOR_PAIR(7));
    mvwprintw(win, 4, 4, "OILP   %.2f bar", oilPressure);
    wattroff(win, COLOR_PAIR(7));

    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 4, 61-17, "SPEED  %3d km/h", speed);
    wattroff(win, COLOR_PAIR(2));

    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, 6, 4, "[ RPM: %.0f ]", rpm);
    wattroff(win, COLOR_PAIR(3));

    draw_shiftLights(win, 0, rpm);

    wrefresh(win);
}

void init_colors(){
    initscr();
    noecho();
    curs_set(FALSE);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, -1);   // Texte normal
    init_pair(2, COLOR_GREEN, -1);  // Vitesse
    init_pair(3, COLOR_YELLOW, -1); // RPM
    init_pair(4, COLOR_RED, -1);    // Bargraph (non utilisé ici)
    init_pair(5, COLOR_BLUE, -1);   // Eau
    init_pair(6, COLOR_CYAN, -1);   // Boost
    init_pair(7, COLOR_MAGENTA, -1);// Huile
}

void dashboard_loop(WINDOW *win){
    int ch, started = 0, gear = 0, speed = 0;
    int old_gear = 0;
    float throttle = 0.0f;
    float rpm = 800.0f;
    int throttle_held = 0;
    float boost = -0.4f;
    float T_water = 25.0f;
    float oilPressure = 1.0f;

    const float rpm_min = 800.0f;
    const float rpm_max = 7000.0f;

    while ((ch = getch()) != 'q') {
        if (ch == 's' && !started) {
            draw_shiftLights(win, 1, 0);
            started = 1;
        }

        if (!started) continue;

        old_gear = gear;

        if (ch == KEY_RIGHT && gear < MAX_GEAR) {
            gear++;
        }
        if (ch == KEY_LEFT && gear > MIN_GEAR) {
            gear--;
        }

        if (gear >= 1 && old_gear != gear) {
            float ratio_old = gear_ratio[old_gear - 1];
            float ratio_new = gear_ratio[gear - 1];
            rpm *= (ratio_new / ratio_old);
            if (rpm > rpm_max) rpm = rpm_max;
            if (rpm < rpm_min) rpm = rpm_min;
        }

        throttle_held = (ch == KEY_UP);
        if (throttle_held) {
            throttle += 0.008f;
            if (throttle > 1.0f) throttle = 1.0f;
        } else {
            throttle -= 0.002f;
            if (throttle < 0.0f) throttle = 0.0f;
        }

        float target_rpm = rpm;

        if (gear == 0) {
            target_rpm = rpm_min + throttle * (rpm_max - rpm_min);
            speed = 0;
        } else if (gear >= 1) {
            float ratio = gear_ratio[gear - 1];
            speed += throttle * 2.5f;
            speed -= 0.05f;
            if (speed < 0.0f) speed = 0.0f;

            float max_speed = rpm_max / (ratio * RPM_FACTOR);
            if (speed > max_speed) speed = max_speed;

            target_rpm = speed * ratio * RPM_FACTOR;
            if (target_rpm > rpm_max) target_rpm = rpm_max;
            if (target_rpm < rpm_min) target_rpm = rpm_min;
        }

        float rpm_interp_factor = 0.08f;
        rpm += (target_rpm - rpm) * rpm_interp_factor;

        boost = get_boost(rpm, throttle, boost, 1.5f, 0.05f);
        T_water = get_WaterTemp(T_water, rpm, throttle, 25.0f, 0.1f);
        oilPressure = get_oil_pressure(rpm, oilPressure, 5.5f, 0.05f);

        draw_info_panel(win, rpm, throttle, gear, speed, boost, T_water, oilPressure);
        wait_ms(20);
    }
}

void init_dashboard(){
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    init_colors();
    set_border(stdscr, LINES);
    dashboard_loop(stdscr);
}