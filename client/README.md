Game Design derp
============================

Game documentation and Protocol can be found here:  
https://goo.gl/PLfIP5

### Engine

Game depends on SDL2, SDL2_image and SDL2_ttf.

Easiest install of depends on apt-get:
```
sudo apt-get install libsdl2-2.0-0 \
libsdl2-image-2.0-0  libsdl2-ttf-2.0-0\
libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev \
mesa-utils mesa-common-dev
```

### Makefile
 * make: create objects -if needed- and links program
 * make remake: recreates all objects, removes bin folder and makes
 * make run: calls make and the runs bin/program
 * make clean: cleans objects and makes
 * make cleaner: removes bin folder, cleans objects and remakes

Makefile has option of either building with static or shared libraries. For classroom 6208 static works, so it is default. If one wants to swap, uncomment line 25 and comment line 27.

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
+- Makefile
+- README.md    //
```

#### References

For smaller things or snippets, reference (web link) can be found in code. Major sources of information for client coding are:

[Lazy Foo's Beginning Game Programming v2.0:](http://lazyfoo.net/tutorials/SDL/index.php#Hello SDL) Most of the SDL stuff is learned from here

[TwinklebeadDev SDL2.0 Tutorials:](http://www.willusher.io/pages/sdl2/) Some basic SDL2 stuff, events and ttf  

[MakingGamesWithBen Advanced C++/Graphics Tutorial:](https://www.youtube.com/watch?v=FxCC9Ces1Yg&list=PLSPw4ASQYyymu3PfG9gxywSPghnSMiOAW) Mostly OpenGl stuff but some of the game engine concepts are adopted from here

[Beej's Guide to Network Programming:](http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html) All of the network programming
