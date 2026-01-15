@echo off
setlocal

echo Starting build...
sighmake SnakeGame.buildscript -g Makefile || pause

echo Build completed successfully.
pause
