# HI-GUI

Hi! First release under development.


Description:
HiGUI, a groundbreaking GUI library that simplifies interface design for both non-coders and developers. Users will customize interfaces through a high-level config file, bypassing coding complexities. 

Developers benefit from:
- User-friendly C++ GUI developing, 
- Advanced animation support,
- GLSL shader integration. 

HiGUI features:
- Image display from URLs, 
- GUI builder, 
- 3D.

hiGUI objects mimic HTML pages with CSS styling, offering vast creative and design possibilities. 
My goal is to blend simplicity with advanced functionality in UI design.

## Build from sources
First, install [Python](https://www.python.org/) (python version >=3.7) and [CMake](https://cmake.org/) if they are not already installed.

```bash
pip3 install jinja2
```

## Build
### Common Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/higui-org/hi-gui
   cd hi-gui
   ```

2. Create a build directory and navigate into it:
   ```bash
   mkdir build && cd build
   ```

3. Configure the project using CMake:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   cmake --build .
   ```

### Linux (Ubuntu, Fedora, Arch, etc.)
#### Dependencies
- Ubuntu:
  ```bash
  sudo apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
  ```

- Fedora:
  ```bash
  sudo dnf install libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
  ```

- Arch:
  ```bash
  sudo pacman -S libxrandr libxinerama libxcursor libxi
  ```

### MacOS
Install Xcode and Xcode Command Line Tools:
```bash
xcode-select --install
```

### Windows
1. Ensure Visual Studio is installed with C++ support.
2. Use CMake GUI to configure and generate Visual Studio project files.
3. Open the generated project file in Visual Studio and build the project.
