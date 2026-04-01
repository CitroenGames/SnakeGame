# SnakeGame

A classic Snake game built with C++20 and SDL3.

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
![SDL3](https://img.shields.io/badge/SDL-3.4.0-green)

## Controls

| Key | Action |
|-----|--------|
| WASD / Arrow Keys | Move snake |
| R | Restart after game over |

## Building

This project uses [sighmake](https://github.com/CitroenGames/sighmake) as its build system generator. Install sighmake first, then:

### Windows

```batch
# Generate Visual Studio solution
Generate.bat

# Or from command line
sighmake SnakeGame.buildscript

# Build
sighmake --build . --config Release
```

### Linux

```bash
# Install SDL3 development libraries
sudo apt install libsdl3-dev  # Debian/Ubuntu

# Generate Makefile
./generate.sh

# Build
sighmake --build . --config Release
```

## Project Structure

```
SnakeGame/
├── src/
│   └── main.cpp
├── 3rdparty/
│   └── SDL3-3.4.0/          # Pre-built SDL3 (Windows)
├── SnakeGame.buildscript     # Main build configuration
├── Generate.bat              # Windows project generation
├── generate.sh               # Linux Makefile generation
└── usage.md                  # sighmake documentation
```

## Features

- Grid-based movement
- Screen wrapping
- Self-collision detection
- Score tracking in window title
- Cross-platform (Windows & Linux)
