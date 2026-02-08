# Projectile Motion Simulator

A physics simulation program that calculates and visualizes projectile trajectories with optional air resistance.

## Features

- ✨ Real-time trajectory visualization
- 📊 Physics calculations (max height, range, flight time)
- 🌬️ Toggle air resistance on/off
- 🎮 Interactive controls to adjust parameters
- 🌍 Multiple gravity presets (Earth, Moon, Mars)
- 🎯 Animated projectile motion

## Physics Implementation

### Without Air Resistance
Uses analytical kinematic equations:
- `x = v₀ × cos(θ) × t`
- `y = v₀ × sin(θ) × t - ½ × g × t²`

### With Air Resistance
Numerical simulation using drag force:
- `F_drag = ½ × ρ × Cd × A × v²`
- Updates velocity and position each timestep

## Prerequisites

You need to install **SFML** (Simple and Fast Multimedia Library) for graphics.

### Windows Installation:

**Option 1: Using MSYS2 (Recommended)**
```bash
# Install MSYS2 from https://www.msys2.org/
# Then in MSYS2 terminal:
pacman -S mingw-w64-x86_64-sfml
```

**Option 2: Manual Installation**
1. Download SFML from https://www.sfml-dev.org/download.php
2. Extract to `C:\SFML`
3. Add SFML bin folder to system PATH

### Linux Installation:
```bash
sudo apt-get install libsfml-dev
```

### macOS Installation:
```bash
brew install sfml
```

## Compilation

### Using g++ directly:

**Windows (MSYS2):**
```bash
g++ -std=c++17 projectile_simulator.cpp -o projectile_simulator.exe -IC:/msys64/mingw64/include -LC:/msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system
```

**Linux/macOS:**
```bash
g++ -std=c++17 projectile_simulator.cpp -o projectile_simulator -lsfml-graphics -lsfml-window -lsfml-system
```

### Using the build task:

Create a custom task in `.vscode/tasks.json`:
```json
{
    "label": "Build SFML Project",
    "type": "shell",
    "command": "g++",
    "args": [
        "-std=c++17",
        "${file}",
        "-o",
        "${fileDirname}\\output\\${fileBasenameNoExtension}.exe",
        "-IC:/msys64/mingw64/include",
        "-LC:/msys64/mingw64/lib",
        "-lsfml-graphics",
        "-lsfml-window",
        "-lsfml-system"
    ]
}
```

## Usage

1. Run the program:
```bash
./projectile_simulator
```

2. Enter initial parameters:
   - Initial velocity (m/s)
   - Launch angle (degrees)
   - Air resistance (y/n)

3. Use keyboard controls in the visualization window:

### Controls:

| Key | Action |
|-----|--------|
| **SPACE** | Start animation |
| **R** | Recalculate trajectory and print results |
| **A** | Toggle air resistance ON/OFF |
| **↑ / ↓** | Increase/decrease launch angle |
| **← / →** | Decrease/increase velocity |
| **G** | Cycle through gravity presets (Earth 9.8, Moon 1.6, Mars 3.7) |

## Example Output

```
=== Projectile Motion Simulator ===
Initial Velocity: 50 m/s
Launch Angle: 45°
Gravity: 9.8 m/s²
Air Resistance: OFF

Results:
├─ Max Height: 63.78 m
├─ Range: 255.10 m
└─ Flight Time: 7.21 s
```

## Code Structure

- **Vector2D**: Simple 2D vector structure
- **ProjectileData**: Stores simulation parameters
- **ProjectileSimulator**: Core physics engine
  - `calculateAnalytical()`: No air resistance
  - `calculateNumerical()`: With air resistance
- **Visualizer**: SFML-based graphics rendering

## Learning Points

This project demonstrates:
- Kinematic equations
- Numerical integration (Euler method)
- Vector mathematics
- Drag force calculations
- Real-time graphics with SFML
- Object-oriented design in C++

## Future Enhancements

- [ ] Add wind effects
- [ ] Target practice mode
- [ ] Multiple projectiles comparison
- [ ] 3D visualization
- [ ] Export trajectory data to CSV
- [ ] Velocity vector visualization
- [ ] Energy graphs (kinetic + potential)

## License

MIT License - Feel free to use for learning and portfolio projects!

## Author

Built with ❤️ for learning physics and C++ graphics programming
