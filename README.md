# Air hockey
### The C++ project with SDL library of Microsoft Visual Studio.
#### What to do to run the project:
1) Clone the repository to you OS directory (for example 'C:\projects\air_hockey').
2) Download SDL library and its components (Development Libraries) unzip and merge into one directory. For example 'C:\SDL2':
   * [SDL2](https://www.libsdl.org/download-2.0.php)
   * [SDL image](https://www.libsdl.org/projects/SDL_image/)
   * [SDL mixer](https://www.libsdl.org/projects/SDL_mixer/)
   * [SDL ttf](https://www.libsdl.org/projects/SDL_ttf/)
3) Copy all *.dll files from 'C:\SDL2\lib\x86 (or x64)' to project direcectory (C:\projects\air_hockey).
4) MSVS project must have the following configurations - Project -> air_hockey Properties -> Configuration Properties ->:
- a) C/C++ -> General -> Additional Include Directories - <Edit...> directory with includes of SDL2, for examle:
    C:\SDL2\include
- b) Linker -> General -> Additional Library Directories - <Edit...> directory with libraries of SDL2 x86 or x64, for examle:
    C:\SDL2\lib\x86
- c) Linker -> Input -> Additional Dependencies - <Edit...> include following libraries:  
    SDL2.lib  
    SDL2main.lib  
    SDL2_image.lib  
    SDL2_ttf.lib  
    SDL2_mixer.lib  

#### Control in the game:
- quit the game - "q" or closed the window
- on/off sound - "m" or LMB on the speaker  
(only on the main menu):
- start the game - "Enter" or LMB on 'START'
- change difficulty - "Left" or "Right" or LMB on the difficulty  
(only during the game):
- pause - "p"
- return to the main menu - RMB or "Esc"
- control the bit-player - move the mouse

### Sample

![](https://github.com/vgladush/resources/blob/master/air_hockey/air_hockey.gif)
