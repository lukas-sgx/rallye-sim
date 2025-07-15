void init_dashboard();
void init_colors(); 
void set_border(WINDOW *win, int lines);
void* draw_shiftLights(void* arg);
void draw_rpm_bargraph(int rpm);
void draw_info_panel(); 
void dashboard_loop(WINDOW *win);