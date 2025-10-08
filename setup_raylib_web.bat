@echo off
echo Setting up raylib for WebAssembly...

echo Cleaning up previous attempts...
rmdir /s /q raylib_src 2>nul

echo Downloading raylib source code...
git clone https://github.com/raysan5/raylib.git raylib_src

echo Setting up Emscripten...
call C:\Users\isiva\emsdk\emsdk_env.bat

echo Building raylib for web...
cd raylib_src\src

echo Cleaning previous build...
rmdir /s /q build_web 2>nul
mkdir build_web
cd build_web

echo Configuring with Ninja...
emcmake cmake .. -DBUILD_EXAMPLES=OFF -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -G "Ninja"

echo Compiling raylib...
emmake ninja

echo.
if exist libraylib.a (
    echo SUCCESS! Raylib for web built successfully!
) else (
    echo Raylib build may have failed.
)
echo.
pause