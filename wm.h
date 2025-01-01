#ifndef WM_H
#define WM_H

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>


#define BORDER_WIDTH 5


class WindowManager {
private:
    int resize_direction;

    bool is_resizing;
    bool is_on_border;
    bool is_dragging;

    int start_x, start_y;
    int delta_x, delta_y;

    Display *display;
    Window window;
    Window root_window;
    Window title_bar;
    
    int screen;

    Colormap color_map;
    GC graphics_ctx;

    // cursors
    Cursor x_resize_cursor;
    Cursor y_resize_cursor;

    XRenderPictFormat *pict_format;
    
    void composite_init();
    void gl_init();
    XWindowAttributes get_win_attrs();
    void check_resize_request(XWindowAttributes *attrs, int x, int y);

public:
    WindowManager();
    ~WindowManager();

    int init(const char* title);
    void create_window(int width, int height, int x, int y);
    void start_loop();
    void stop_loop();
};

#endif // WM_H
