#!/bin/bash

echo "Starting build..."
sighmake SnakeGame.buildscript -g makefile || exit 1

echo "Build completed successfully."
