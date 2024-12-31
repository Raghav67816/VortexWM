
# VortexWM

A window manager inspired by Iron Man. You might think what is the relation between a window manager and Iron Man. If you see Iron Man 1, you will find Tony working on his computer using a pen to drag files. In that moment you can notice blue glowing windows floating on his computer.

Even though this is not the complete OS but a window manager that tries to replicate that particular scene and it's functional.

![](https://raw.githubusercontent.com/Raghav67816/VortexWM/refs/heads/main/image.png)

**Looks like a black screen but took a lot of efforts. phewww**

## Features

- Shining blue border
- Custom shortcuts
- Title bar inspired by sci-fi movies.
- And it's usable

**Note: This is currently under development**

## Installation
To install this window manager as a standalone application. Go to releases page and download the "wm" executable because that's the stable executable I have.
To build from source.

1. Install dependencies
```bash
sudo apt update
sudo apt install build-essential libx11-dev libxcomposite-dev libxrender-dev libxtst-dev libxrandr-dev libxi-dev libxfixes-dev libxcb1-dev libxmu-dev
```

2. Download repo files
```bash
git clone <repo-link>
```

3. Compile
```bash
g++ wm.cpp main.cpp -lX11 -lXrender -lXcomposite
```

Compiling might fail with the current code. I will fix it soon :) Ahh... the xlib documentation is just hectic.


### 1. **Window Manager Core**
- [x] **Create an event loop** that listens for user actions such as window creation, resizing, and moving.
- [x] **Implement window creation** with basic X11 calls (`XCreateWindow`).
- [x] **Handle window events** using `XSelectInput` for key presses, button presses, and other events.

### 2. **Window Decorations**
- [ ] **Create a title bar** for windows that includes:
    - [x] Basic text display (window title).
    - [ ] Buttons for minimizing, maximizing, and closing the window.
- [ ] **Make title bar draggable** for window movement.

### 3. **Window Resizing & Moving**
- [ ] **Add functionality for resizing windows** using mouse drag events.
- [ ] **Implement window movement** by detecting mouse drag on the title bar.

### 4. **Transparency (XRender/XComposite)**
- [ ] **Implement transparent title bars** using XRender extension.
- [ ] **Use XComposite for compositing** to add window shadows and other visual effects.
- [ ] **Create a translucent background for windows** using proper color settings.

### 5. **Compositing and Effects**
- [ ] **Initialize and configure Xcomposite extension** to enable compositing effects.
- [ ] **Create and apply window shadows** using XRender.
- [ ] **Add window animations** like growing window height using the Xcomposite extension.
  
### 6. **Keyboard Shortcuts**
- [ ] **Integrate keyboard shortcuts** using XKB extension for window actions like:
    - [ ] Close window.
    - [ ] Minimize window.
    - [ ] Switch between windows.

### 7. **User Interface (UI)**
- [ ] **Build basic UI for window manager controls** (e.g., window list, settings).
- [ ] **Implement user-friendly interface for window management**, using components like taskbars and system trays.
- [ ] **Develop window preview functionality** to show thumbnails or minimized views of open windows.

And yes I got this list generated by ChatGPT.
I really excited to tick all the check-boxes.
