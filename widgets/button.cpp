#include "button.h"
#include <string>
#include <iostream>
#include <functional>
#include <X11/Xlib.h>

using namespace std;

Button::Button(Display *display_, Window parent_win_, std::string text_){
    text = text_;
    display = display_;
    parent_win = parent_win_;
    Button_ btn = {};
}

void Button::set_text(std::string &text_){
    btn.text = text_;
}

void Button::set_color(XColor color_){
    color = color_;
    btn.color = color_;
}

void Button::set_on_hover_color(XColor color_){
    btn.on_hover_color = color_;

}

void Button::set_handler(function<void()> action){
    btn.action = action;
}

void Button::draw_btn_frame(int h, int w){
    XColor bg_color;
    bg_color.red = 0;
    bg_color.green = 0;
    bg_color.blue = 0;


    GC frame_gc = XCreateGC(display, parent_win, 0, NULL);
    XSetForeground(
        display, frame_gc, bg_color.pixel
    );
    XDrawRectangle(
        display,
        parent_win,
        frame_gc,
        10, 10,
        w, h
    );

    XColor white;
    white.red = 255;
    white.blue = 255;
    white.green = 255;

    XSetForeground(
        display, frame_gc, white.pixel
    );
    XDrawString(
        display,
        parent_win,
        frame_gc,
        0, 0,
        text.c_str(),
        text.length()

    );
    XFreeGC(display, frame_gc);
}

Button::~Button(){
    cout << "button delted" << endl;
}
