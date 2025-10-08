@echo off
echo Downloading pre-built raylib for web...
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/raw/master/projects/Web/raylib.html' -OutFile 'raylib_template.html'"
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/raw/master/projects/Web/raylib.js' -OutFile 'raylib.js'"
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/raw/master/projects/Web/raylib.wasm' -OutFile 'raylib.wasm'"

echo Creating web version of your game...
call C:\Users\isiva\emsdk\emsdk_env.bat

echo Compiling your game for web...
emcc -o game.html main.cpp ^
 -s USE_GLFW=3 ^
 -s ASYNCIFY ^
 -s ALLOW_MEMORY_GROWTH=1 ^
 -s MAX_WEBGL_VERSION=2 ^
 -s MIN_WEBGL_VERSION=2 ^
 -s FORCE_FILESYSTEM=1 ^
 -DPLATFORM_WEB ^
 -O2 ^
 --preload-file assets ^
 -std=c++17

echo.
if exist game.html (
    echo SUCCESS! Your game compiled for web!
    echo Open game.html in your browser!
) else (
    echo Compilation failed. Let's try a simpler approach...
)
pause