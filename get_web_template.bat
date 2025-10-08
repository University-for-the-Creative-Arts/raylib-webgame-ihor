@echo off
echo Downloading raylib web template...
curl -L -o web_template.zip https://github.com/raysan5/raylib/archive/refs/heads/master.zip

echo Extracting template...
powershell -Command "Expand-Archive -Path web_template.zip -DestinationPath . -Force"

echo Setting up the web project...
xcopy "raylib-master\projects\Web\*" "web_build\" /E /I /Y

echo Cleaning up...
del web_template.zip
rmdir /s /q raylib-master

echo Web template ready in 'web_build' folder!
pause