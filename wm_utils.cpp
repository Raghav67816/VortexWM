#include <iostream>
#include "wm_utils.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <canberra.h>

void play_sound(char sound_type)
{
    ca_context *context;
    if (!ca_context_create(&context))
    {
        cout << "unable to create sound system" << endl;
        // this will not stop the application process.
        return;
    }

    int conn = ca_context_open(context);
    if (!conn)
    {
        cout << "cannot connect to backend service" << endl;
        return;
    }

    ca_context_play(
        context, 0,
        CA_PROP_MEDIA_FILENAME,
        sound_files[sound_type],
        CA_PROP_CANBERRA_CACHE_CONTROL,
        "permanent",
        NULL
    );
    ca_context_destroy(context);
}

void render_opengl(Display* display, Window window, float rotation_angle) {
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply rotation
    glRotatef(rotation_angle, 1.0f, 1.0f, 0.0f);

    // Render the cube
    glBegin(GL_QUADS);

    // Front face
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

    // Other faces...

    glEnd();
    glXSwapBuffers(display, window);
}