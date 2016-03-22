# PatBoy
Game Boy emulator written in C++ with SDL2.

<p align="center">
  <img src="http://jonathanmcontreras.com/images/portfolio/patboy-tetris.png" alt="Tetris"/>
</p>
# Specification
PatBoy follows GameBoy specification that can be found in the pandocs as well as in the documentation provided by nintendo in the Game Boy Programming manual. The documentation needed for building a game boy emulator can be found in: http://bgb.bircd.org/pandocs.htm.

# Dependencies

Patboy is using *SDL2* to render the emulator. We need to install SDL2 for Mac OS manually. Google Test is being used as test library for the project.

### How to install SDL2

SDL2 can be installed following this tutorial from the official page: https://wiki.libsdl.org/Installation. Some versions of the library need to be properly sign using the next commands, if we don't do it Xcode will crash while we are trying to run our application.

```shell
codesign -f -s - SDL2
```

### How to install GoogleTest

Google Test can be found in the following repository : https://github.com/google/googletest