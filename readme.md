![build](https://github.com/mattiasgustavsson/svgs/actions/workflows/main.yml/badge.svg)

# svgs
"Super Video Game System". Fantasy home computer. Work-in-progress. (https://twitter.com/hashtag/supervgs)

Build instructions
------------------

### Windows
From a Visual Studio command prompt do:
```
cl source\main.c
```

### Mac
```
clang source/main.c -lSDL2 -lGLEW -framework OpenGL
```

SDL2 and GLEW are required - if you don't have then installed you can do so by running
```
brew install sdl2 glew
```

### Linux
```
gcc source/main.c -lSDL2 -lGLEW -lGL -lm -lpthread
```

SDL2 and GLEW are required - if you don't have them installed you can do so by running
```
sudo apt-get install libsdl2-dev
sudo apt-get install libglew-dev
```


