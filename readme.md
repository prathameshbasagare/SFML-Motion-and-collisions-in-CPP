## SFML Polymorphic Shapes

A minimal C++/SFML demo that loads shapes from a config file and animates them with per-shape velocities, edge collisions, and centered text labels. Circles and rectangles are handled by a single unified `Shape` class.

## Features
- Unified `Shape` class for circles and rectangles
- Config-driven window and shapes (`config.txt`)
- Per-shape velocity and wall-bounce collisions
- Centered labels with configurable font, size, and color
- Simple keyboard control for inverting velocities

## Requirements
- C++ compiler with C++17 support
- SFML (graphics, window, system, audio)
- OpenGL (linked via `-lGL` on Linux)

Note: The repository includes ImGui and ImGui-SFML sources already linked in the `Makefile`.

## Build
```bash
make
```
Outputs the binary at `./bin/sfmlgame`.

## Run
```bash
./bin/sfmlgame
```

## Configuration
The app reads `config.txt` at the project root. Example (provided):
```txt
Window 800 600
Font fonts/Roboto-Black.ttf 18 255 255 255
Circle CGreen 100 100 -0.03 0.02 0 255 0 50
Circle CBlue 200 200 0.02 0.04 0 0 255 100
Circle CPurple 300 300 -0.02 -0.01 255 0 255 75
Rectangle RRed 200 200 0.1 0.15 255 0 0 50 25
Rectangle RGrey 300 250 -0.02 0.02 100 100 100 50 100
Rectangle RTeal 25 100 -0.02 -0.02 0 255 255 100 100
```

### Grammar
- `Window <width> <height>`
- `Font <path> <size> <r> <g> <b>`
- `Circle <name> <x> <y> <vx> <vy> <r> <g> <b> <radius>`
- `Rectangle <name> <x> <y> <vx> <vy> <r> <g> <b> <width> <height>`

Coordinates are in pixels. Velocities are in pixels per frame. Colors are 0–255.

## Controls
- `X`: invert both X and Y velocities for all shapes
- Window close button: exit

## Notes
- Frame rate is limited via `window.setFramerateLimit(1000)`; adjust as needed.
- Text labels are centered within each shape using local bounds and shape geometry.

## Project Structure
- `src/main.cpp`: application entry and the unified `Shape` implementation
- `config.txt`: window, font, and shape definitions
- `bin/`: output binary and runtime assets
- `src/imgui`, `src/imgui-sfml`: third-party sources already wired in the `Makefile`

## License
This project embeds third-party libraries (ImGui, ImGui-SFML). Refer to their respective licenses in `src/imgui/LICENSE.txt` and `src/imgui-sfml/LICENSE`.


