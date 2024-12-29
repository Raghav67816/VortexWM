#include "wm.h"
#include <stdio.h>
#include <iostream>
#include "wm_visual.h"
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xcomposite.h>

using namespace std;

WindowManager::WindowManager()
{
    display = nullptr;
    screen = 0;
    window = NULL;
    color_map = NULL;
    root_window = NULL;
    x_resize_cursor = NULL;
    y_resize_cursor = NULL;
    is_resizing = false;
    is_on_border = false;
    resize_direction = 0;
    is_dragging = false;
    wm_info info = {};
}

int WindowManager::init(const char *title)
{
    display = XOpenDisplay(NULL);
    if (!display)
    {
        printf("Failed to open display\n");
        return 1;
    }

    screen = DefaultScreen(display);
    root_window = DefaultRootWindow(display);

    color_map = DefaultColormap(display, screen);
    graphics_ctx = XDefaultGC(display, screen);

    // pre-define cursor
    x_resize_cursor = XCreateFontCursor(display, XC_sb_h_double_arrow);
    y_resize_cursor = XCreateFontCursor(display, XC_sb_v_double_arrow);

    // init compositor
    VisualMgr visual_mgr;
    bool is_comp_compat = visual_mgr.init_compositor(display);
    if (!is_comp_compat)
    {
        exit(1);
    }
    else
    {
        return 0;
    }
}

void WindowManager::create_window(int width, int height, int x, int y)
{
    XSetWindowAttributes attrs;
    attrs.background_pixel = 0;
    attrs.colormap = color_map;
    attrs.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | FocusChangeMask | PointerMotionMask | ButtonReleaseMask;
    attrs.border_pixel = 0;
    attrs.override_redirect = True;

    window = XCreateWindow(
        display, root_window,
        x, y, width, height, 5,
        CopyFromParent, InputOutput, CopyFromParent,
        CWBackPixel | CWColormap | CWEventMask | CWBorderPixel | CWOverrideRedirect,
        &attrs);

    if (window == 0)
    {
        printf("Failed to create window\n");
        return;
    }

    XCompositeRedirectWindow(display, window, CompositeRedirectAutomatic);
}

void WindowManager::draw_border()
{
    XColor cyan;
    if (!XParseColor(display, color_map, "cyan", &cyan))
    {
        printf("Failed to parse color\n");
    }

    if (!XAllocColor(display, color_map, &cyan))
    {
        printf("Failed to allocate color\n");
    }

    XWindowAttributes temp_attrs;
    XGetWindowAttributes(display, window, &temp_attrs);

    GC border_gc = XCreateGC(display, window, 0, NULL);
    XSetForeground(display, border_gc, cyan.pixel);
    XDrawRectangle(display, window, border_gc, 0, 0, temp_attrs.width - 1, temp_attrs.height - 1);

    XFreeGC(display, border_gc);
}

void WindowManager::check_resize_request(XWindowAttributes *attrs, int x, int y)
{

    is_on_border = false;

    // left border
    if (x <= attrs->width - 1 && x >= attrs->width - 10)
    {
        printf("right border");
        XDefineCursor(display, window, x_resize_cursor);
        is_on_border = true;
        resize_direction = 1;
    }

    // right border
    if (x <= 10 && x >= 0)
    {
        printf("left border");
        XDefineCursor(display, window, x_resize_cursor);
        is_on_border = true;
        resize_direction = 1;
    }

    // bottom border
    if (y <= 10 && y >= 0)
    {
        printf("upper border");
        XDefineCursor(display, window, y_resize_cursor);
        is_on_border = true;
        resize_direction = 2;
    }

    if (y >= attrs->height - 10 && y <= attrs->height - 1)
    {
        printf("bottom border\n");
        XDefineCursor(display, window, y_resize_cursor);
        is_on_border = true;
        resize_direction = 2;
    }

    if (!is_on_border)
    {
        XUndefineCursor(display, window);
        resize_direction = 0;
    }
}

void WindowManager::start_loop()
{
    XMapWindow(display, window);
    XSetInputFocus(display, window, RevertToNone, CurrentTime);

    XWindowAttributes global_attrs;
    XGetWindowAttributes(display, window, &global_attrs);

    XEvent event;

    while (1)
    {
        XNextEvent(display, &event);
        draw_border();

        switch (event.type)
        {
        case Expose:
            std::cout << "window exposed" << std::endl;
            break;

        case ButtonPress:
            if (is_on_border && event.xbutton.button == Button1)
            {
                is_resizing = true;
            }
            break;

        case ButtonRelease:
            if (event.xbutton.button == Button1 && is_resizing)
            {
                is_resizing = false;
            }
            break;

        case KeyPress:
        {
            std::cout << "Key pressed!" << std::endl;
            KeySym key_symbol = XLookupKeysym(&event.xkey, 0);
            unsigned int modifier = event.xkey.state;
            modifier = modifier & ~(LockMask | Mod2Mask);
        }

        case MotionNotify:
            if (is_resizing)
            {
                XWindowAttributes new_attrs;
                XGetWindowAttributes(display, window, &new_attrs);

                if (resize_direction == 1)
                {
                    int new_width = event.xmotion.x;
                    if (new_width > 0)
                    {
                        XResizeWindow(display, window, new_width, new_attrs.height);
                    }
                }
                else if (resize_direction == 2)
                {
                    int new_height = event.xmotion.y;
                    if (new_height > 0)
                    {
                        XResizeWindow(display, window, new_attrs.width, new_height);
                    }
                }
            }
            else
            {
                check_resize_request(&global_attrs, event.xmotion.x, event.xmotion.y);
            }
            break;

        case ConfigureNotify:
            std::cout << "user wants to change window props" << std::endl;
            break;

        default:
            break;
        }
    }

    XFlush(display);
}

void WindowManager::stop_loop()
{
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

wm_info WindowManager::get_wm(){
    info.display = display;
    info.screen = screen;
    info.window = window;
    info.root_window = root_window;

    return info;
}

WindowManager::~WindowManager()
{
    printf("Destroyed the window\n");
}
