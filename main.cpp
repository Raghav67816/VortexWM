#include "wm.h"

int main() {
    // Create an instance of WindowManager
    WindowManager wm;

    // Initialize the window manager
    wm.init("window");

    // Create the window with specified dimensions and position
    wm.create_window(800, 600, 100, 100);

    // Start the event loop
    wm.start_loop();

    // After the loop ends, stop the event loop and clean up
    wm.stop_loop();

    return 0; // Exit the program successfully
}
