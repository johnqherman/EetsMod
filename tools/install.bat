@echo off
rem install the eetsmod loader + bundled mods into an Eets install. Run from the extracted release dir.
rem   install.bat [GAME_DIR]
setlocal
cd /d "%~dp0"
set "G=%~1"
if "%G%"=="" set "G=%ProgramFiles(x86)%\Steam\steamapps\common\Eets"
if not exist "%G%\Eets.exe" (
  echo no Eets.exe at "%G%"  ^(pass the game dir as the first argument^)
  exit /b 1
)
copy /Y version.dll "%G%\" >nul
if not exist "%G%\eetsmod-include" mkdir "%G%\eetsmod-include"
if not exist "%G%\mods" mkdir "%G%\mods"
copy /Y eetsmod-include\*.h "%G%\eetsmod-include\" >nul
copy /Y mods\*.eetsmod "%G%\mods\" >nul 2>nul
echo installed (Windows). No launch option needed - version.dll loads automatically.
echo Launch Eets normally.
endlocal
