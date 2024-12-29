#ifndef WM_H
#define WM_H

#include <X11/Xlib.h>
#include "wm_visual.h"


#define BORDER_WIDTH 5

struct wm_info{
    Display *display;
    int screen;
    Window window;
    Window root_window;
};


class WindowManager {
private:
    int resize_direction;

    bool is_resizing;
    bool is_on_border;
    bool is_dragging;


    Colormap color_map;
    GC graphics_ctx;

    // cursors
    Cursor x_resize_cursor;
    Cursor y_resize_cursor;

    VisualMgr visual_mgr;

    void draw_border();
    void composite_init();
    wm_info get_wm();
    void check_resize_request(XWindowAttributes *attrs,int x, int y);

public:
    Display *display;
    Window window;
    Window root_window;
    int screen;

    wm_info info;

    WindowManager();
    ~WindowManager();

    int init(const char* title);
    void create_window(int width, int height, int x, int y);
    void start_loop();
    void stop_loop();
    XWindowAttributes get_win_attrs();
};

#endif // WM_H
