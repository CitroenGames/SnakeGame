@echo off
setlocal

echo Starting build...
sighmake SnakeGame.buildscript || pause

echo Build completed successfully.
pause
