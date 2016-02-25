# PatBoy
Game Boy emulator written in C++ with SDL2 and OpenGL.

# Specification
PatBoy follows GameBoy specification that can be found in the pandocs as well as in the documentation provided by nintendo in the Game Boy Programming manual. The documentation needed for building a game boy emulator can be found in: http://bgb.bircd.org/pandocs.htm.

# Dependencies

Patboy is using *OpenGL* (ver 4.1) and *SDL2*. While OpenGL is included in Xcode development kit, we need to install SDL2 for Mac OS manually. As test library the project uses Google Test.

### How to install SDL2

SDL2 can be installed following this tutorial from the official page: https://wiki.libsdl.org/Installation. Some versions of the library need to be properly sign using the next commands, if we don't do it Xcode will crash while we are trying to run our application.

```shell
codesign -f -s - SDL2
```

### How to install GoogleTest

Google Test can be found in the following repository : https://github.com/google/googletest
