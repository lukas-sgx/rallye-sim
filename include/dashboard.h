void init_dashboard();
void init_colors(); 
void set_border(WINDOW *win, int lines);
void draw_shiftLights(WINDOW *win, int init);
void draw_rpm_bargraph(int rpm);
void draw_info_panel(WINDOW *win, float rpm, float throttle, int gear, int speed, float boostParam, float T_water, float oilPressure);
void dashboard_loop(WINDOW *win);