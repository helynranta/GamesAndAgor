Game Design derp
============================

Agor.io clone, some1 write abstacr

### Engine

Game depends on SDL2, SDL2_image and SDL2_ttf. For possible audio, SDL2_mixer

Easiest install of depends on apt-get:

```
sudo apt-get install libsdl2-2.0-0 \
libsdl2-image-2.0-0  libsdl2-ttf-2.0-0\
libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev \
mesa-utils mesa-common-dev
```
### Folder structure

```
client
+- bin          // compiled program, do not touch
|  +- res       // resources for built program
|  +- {binaries}
+- inc          // includes, headers
|  +- core      // less game specific, some built classes for game
+- lib          // for libraries, plz get rid of by installing deps
+- obj          // built Objects
+- res          // project resource files
|  +- fonts     // font resources, include only true type fonts (TTF)
|  {other resources}
+- src          // source files
|  +- core      // less game specific, more engine
|  {other sources}
+- Game.md      // this document!
+- Makefile
+- README.md    // ?
```

### Makefile
 * make: create objects -if needed- and links program
 * make remake: recreates all objects, removes bin folder and makes
 * make run: calls make and the runs bin/program
 * make clean: cleans objects and makes
 * make cleaner: removes bin folder, cleans objects and remakes

makefiles has 2 options for linking. first option is linking with shared libraries, and second in linking with static libraries. enable one by uncommenting/commenting proper LIB variable (line 25/27)

### Classes

##### GAME SPECIFIC:

Game: takes care of main loop, delta time, frames per second and game logic

Cirle: for all circles (all entitites in game, as they are represented as circles)

Player: kind of circle that player can move
