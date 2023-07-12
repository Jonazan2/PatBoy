# PatBoy
Game Boy emulator written in C++ with SDL2. It supports MBC1, MBC2 and MBC3, RAM saves and has a fully functional debugger.

<p align="center">

  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-mario2.png" alt="DrMario"/>
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-tetris2.png" alt="Tetris"/>
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-zelda.png" alt="Mario2"/>
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-street.png" alt="Mario2"/>
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-metroid2.png" alt="Mario2"/>

</p>


<p align="center">

  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-worldcup.png" alt="DrMario"/>
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-castlevania.png" alt="Tetris"/>
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-drmario.png" alt="Mario2"/>
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-megaman2.png" alt="Mario2"/>
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/patboy-tennis.png" alt="Mario2"/>

</p>

# Specification
PatBoy follows GameBoy specification that can be found in the pandocs as well as in the documentation provided by nintendo in the Game Boy Programming manual. The documentation needed for building a game boy emulator can be found in: http://bgb.bircd.org/pandocs.htm.

# Debugger
The last addition to PatBoy has been a debugger implemented using the library [dearimgui](https://github.com/ocornut/imgui) for the UI. The debugger has breakpoints, step in and run until vsync features implemented as well as memory and video status views.

<p align="center">
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/debugger-updated2.png" alt="PatBoy Debugger"/>
</p>

The video status view allows users to display the content of the VRAM (without the palletes applied for now) and the whole frame buffer (256x256 or 32 by 32 tiles) with a window that indicates the visible area at the time (160x144 or 20 by 18 tiles).


<p align="center">
  <img src="https://github.com/Jonazan2/PatBoy/blob/develop/media/debugger-video.png" alt="PatBoy Debugger Video"/>
</p>

# Dependencies

Patboy is using [SDL2](https://wiki.libsdl.org/Installation) to render the emulator and [OpenGL](https://www.opengl.org/) with [dearimgui](https://github.com/ocornut/imgui) by [Omar Cornut](https://github.com/ocornut) for the debugger. [Google Test](https://github.com/google/googletest) is being used as test library for the project.
