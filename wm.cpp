#include "wm.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <iostream>
#include "wm_visual.h"
#include "wm_utils.h"
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xcomposite.h>

using namespace std;

/*
Window Manager constructor
Initialise all member variables. Leaving them uninitialised leads to mem. leaks
*/
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
    pict_format = nullptr;
    title_bar = NULL;
    start_x, start_y, delta_x, delta_y = 0;
}

/*
WindowManager::init

Setup window manager
create display and define cursors.
connection to X server is made from here
*/
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

    x_resize_cursor = XCreateFontCursor(display, XC_sb_h_double_arrow);
    y_resize_cursor = XCreateFontCursor(display, XC_sb_v_double_arrow);

    XVisualInfo visual_info;
    if (!XMatchVisualInfo(display, screen, 32, TrueColor, &visual_info))
    {
        std::cerr << "No ARGB visual found" << std::endl;
        return 1;
    }

    // init x compositor
    init_xcomp(display);
    pict_format = init_xrender(display, screen);

    return 1;
}

/*
WindowManager::create_window()
if connection to x server was successful create a window
*/

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

    // define title bar
    title_bar = create_title_bar(display, window);
    XCompositeRedirectWindow(display, window, CompositeRedirectAutomatic);
}

void WindowManager::gl_init(){
    GLint attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo *visual_info = glXChooseVisual(display, screen, attributes);
    GLXContext gl_context = glXCreateContext(display, visual_info, NULL, GL_TRUE);
    glXMakeCurrent(display, window, gl_context);
}

/*
WindowManager::check_resize_request

Checks if the mouse is hovering around the border.
*/
void WindowManager::check_resize_request(XWindowAttributes *attrs, int x, int y)
{

    is_on_border = false;

    // left border
    if (x <= attrs->width - 1 && x >= attrs->width - 10)
    {
        XDefineCursor(display, window, x_resize_cursor);
        is_on_border = true;
        resize_direction = 1;
    }

    // right border
    if (x <= 10 && x >= 0)
    {
        XDefineCursor(display, window, x_resize_cursor);
        is_on_border = true;
        resize_direction = 1;
    }

    // bottom border
    if (y <= 10 && y >= 0)
    {
        XDefineCursor(display, window, y_resize_cursor);
        is_on_border = true;
        resize_direction = 2;
    }

    if (y >= attrs->height - 10 && y <= attrs->height - 1)
    {
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

XWindowAttributes WindowManager::get_win_attrs()
{
    XWindowAttributes win_attrs;
    XGetWindowAttributes(display, window, &win_attrs);
    return win_attrs;
}

void WindowManager::start_loop()
{
    XMapWindow(display, window);
    XSetInputFocus(display, window, RevertToNone, CurrentTime);


    XSelectInput(display, window, ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

    XWindowAttributes global_attrs;
    XGetWindowAttributes(display, window, &global_attrs);

    gl_init();

    XEvent event;

    draw_border(
        display,
        window, 
        color_map
    );

    draw_title_bar(
        display, 
        title_bar, 
        pict_format
    );

    set_title(
        display, 
        title_bar, 
        screen,
        "Terminal"
    );

    float rotation_angle = 0.0f;

    while (true)
    {
        XNextEvent(display, &event);

        if (event.xany.window == title_bar)
        {
            switch (event.type)
            {
            case Expose:
                std::cout << "Title Bar Exposed" << std::endl;
                set_title(
                    display,
                    title_bar,
                    screen,
                    "Terminal"
                );
                break;

            case ButtonPress:
                if (event.xbutton.button == Button1)
                {
                    is_dragging = true;
                    start_x = event.xbutton.x_root;
                    start_y = event.xbutton.y_root;
                }
                break;

            case ButtonRelease:
                if (event.xbutton.button == Button1 && is_dragging)
                {
                    is_dragging = false;
                }
                break;

            default:
                break;
            }
        }

        else if (event.xany.window == window)
        {
            switch (event.type)
            {
            case Expose:
                std::cout << "Window Exposed" << std::endl;
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

            case MotionNotify:
                if (is_dragging)
                {
                    delta_x = event.xmotion.x_root - start_x;
                    delta_y = event.xmotion.y_root - start_y;

                    XMoveWindow(display, window, global_attrs.x + delta_x, global_attrs.y + delta_y);
                    XMoveWindow(display, title_bar, global_attrs.x + delta_x, global_attrs.y + delta_y);

                    start_x = event.xmotion.x_root;
                    start_y = event.xmotion.y_root;
                }
                else if (is_resizing)
                {
                    XWindowAttributes new_attrs;
                    XGetWindowAttributes(display, window, &new_attrs);

                    if (resize_direction == 1)
                    {
                        int new_width = event.xmotion.x;
                        if (new_width > 0)
                        {
                            XResizeWindow(display, window, new_width, new_attrs.height);
                            XResizeWindow(display, title_bar, new_width, 30);
                            draw_title_bar(display, title_bar, pict_format);
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
                // glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                draw_border(
                    display,
                    window,
                    color_map
                );
                cout << "redrawing border" << endl;
                break;

            default:
                break;
            }
        }
        rotation_angle += 1.0f;
        if (rotation_angle >= 360.0f) {
            rotation_angle -= 360.0f;
        }
        render_opengl(display, window, rotation_angle);
        XFlush(display);
    }
}


void WindowManager::stop_loop()
{
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

WindowManager::~WindowManager()
{
    printf("Destroyed the window\n");
}
