@echo off
echo Building your cat game for web...
call C:\Users\isiva\emsdk\emsdk_env.bat

echo Compiling...
emcc -o index.html web_game.c ^
 -s USE_GLFW=3 ^
 -s ASYNCIFY ^
 -s ALLOW_MEMORY_GROWTH=1 ^
 -s MAX_WEBGL_VERSION=2 ^
 -s MIN_WEBGL_VERSION=2 ^
 -s FORCE_FILESYSTEM=1 ^
 -DPLATFORM_WEB ^
 -O2 ^
 --shell-file min shell.html

echo.
if exist index.html (
    echo SUCCESS! Your cat game compiled for web!
    echo Open index.html in your browser!
) else (
    echo Compilation failed.
)
echo.
pause