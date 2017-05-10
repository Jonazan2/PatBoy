# PatBoy
Game Boy emulator written in C++ with SDL2.

<p align="center">
  <img src="http://jonathanmcontreras.com/images/portfolio/patboy-tetris.png" alt="Tetris"/>
</p>
# Specification
PatBoy follows GameBoy specification that can be found in the pandocs as well as in the documentation provided by nintendo in the Game Boy Programming manual. The documentation needed for building a game boy emulator can be found in: http://bgb.bircd.org/pandocs.htm.

# Debugger
The last addition to PatBoy has been a debugger implemented using the library [dearimgui](https://github.com/ocornut/imgui) for the UI. The debugger has breakpoints, step in and run until vsync features implemented as well as memory and video status views.

<p align="center">
  <img src="http://jonathanmcontreras.com/images/portfolio/debugger.png" alt="PatBoy Debugger"/>
</p>

# Dependencies

Patboy is using *SDL2* to render the emulator. We need to install SDL2 for Mac OS manually. Google Test is being used as test library for the project.

### How to install SDL2

SDL2 can be installed following this tutorial from the official page: https://wiki.libsdl.org/Installation. Some versions of the library need to be properly sign using the next commands, if we don't do it Xcode will crash while we are trying to run our application.

```shell
codesign -f -s - SDL2
```

### How to install GoogleTest

Google Test can be found in the following repository : https://github.com/google/googletest
