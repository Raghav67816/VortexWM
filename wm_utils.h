#ifndef WM_UTILS_H
#define WM_UTILS_H

#include <canberra.h>
#include <X11/Xlib.h>
#include <map>

using namespace std;

static map<char, char> sound_files = {
    {'expose', './sounds/expose.mp4'}
};

void play_sound(char sound_type);
void render_opengl(Display *display, Window window, float rotation_angle);

#endif // WM_UTILS_H