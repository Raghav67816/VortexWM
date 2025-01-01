#include <string>
#include <X11/Xlib.h>
#include <functional>

struct Button_
{
    std::string text;
    XColor color;
    XColor on_hover_color;
    std::function<void()> action;
};

class Button
{

private:
    Display *display;
    Window parent_win;

    void draw_btn_frame(int h, int w);

public:
    Button_ btn;

    std::string text;
    XColor color;
    XColor on_hover_color;

    Button_ btn;

    Button(Display *display, Window parent_win, std::string text);
    ~Button();

    void set_text(std::string &text_);
    void set_color(XColor color);
    void set_on_hover_color(XColor color);
    void set_handler(std::function<void()> function);
};
