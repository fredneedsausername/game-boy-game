# Game Boy Tic-Tac-Toe

A tic-tac-toe implementation for the original Game Boy, demonstrating an optimized development workflow for retro game development.

## Overview

This project addresses the challenges of Game Boy development by implementing a build system optimized for rapid iteration. Game Boy development presents several constraints:

- 8KB of working RAM
- 4MHz CPU with limited instruction set
- 160x144 pixel display resolution
- Tile-based graphics system
- No standard library or dynamic memory allocation

The toolchain and project structure are designed to minimize build times and provide modern development conveniences.

## Architecture and Build Optimization

### Per-File Compilation Strategy

SDCC (the C compiler for Game Boy) recompiles entire files rather than individual functions. To minimize rebuild times, each function is placed in its own source file.

The `src/separate_files/` directory structure demonstrates this approach:
- `accept_move.c` - 200+ lines of input handling
- `flip_cursor_visibility.c` - 15 lines of sprite toggling
- `handle_win.c` - Win detection and animation logic

With this structure, modifying a small function only triggers recompilation of its specific file, leaving larger modules unchanged. This significantly reduces iteration time on larger projects.

### Toolchain Features

The build system provides several features to streamline development:

**Parallel Compilation**
```bash
make MODE=debug    # Utilizes all available CPU cores
```

**Dual Build Modes**
- `debug`: Includes full symbol information and Emulicious debugger integration with `.cdb` files
- `release`: Optimized production build

**IDE Integration**
- VS Code build and debug tasks configured
- Automatic tool detection and validation
- IntelliSense configured for GBDK headers

## Getting Started

### Prerequisites

1. **GBDK** - Install to `C:/gbdk/`
   - Download from the official GBDK repository
   - Update `GBDK_HOME` in the Makefile if installed to a different location
   - Note: GBDK-2020 has known issues with non-C drive installations and paths containing spaces on Windows

2. **Make for Windows**
   - Available through GnuWin32 or Chocolatey

3. **Emulicious** - For debugging
   - Download from the official Emulicious website

4. **Emulicious Debugger Extension for VS Code** (for IDE integration)
   - Install from the VS Code Extensions Marketplace
   - Required for F5 debugging functionality within VS Code

### Building

```bash
# Debug build (includes debugging symbols)
make MODE=debug

# Release build (optimized)
make MODE=release

# Clean build artifacts for specific mode
make MODE=debug clean
```

Build output locations:
- `bin/debug/game.gb`
- `bin/release/game.gb`

Intermediate files (`.rel`, `.map`, `.cdb`) are organized in `build_files/debug/` or `build_files/release/`.

### Running

**From VS Code:**
1. Press F5
2. Select build mode (debug/release)
3. The ROM builds and launches in Emulicious automatically

**Manual execution:**
```bash
make MODE=debug
# Load bin/debug/game.gb in your preferred emulator
```

### Debugging

Debug builds include complete symbol information. Emulicious debugging capabilities:
- Breakpoint setting by function name
- Variable inspection
- Source-level debugging
- Real-time memory and VRAM viewing

Symbol files (`.cdb`, `.map`, `.noi`) are automatically copied to `bin/debug/` alongside the ROM.

## Makefile Implementation

Key features of the build system:

- **Dependency Tracking**: Auto-generated `.d` files track header dependencies
- **Parallel Execution**: Supports `-j` flag, defaults to `NUMBER_OF_PROCESSORS`
- **Preflight Validation**: Verifies toolchain availability before building
- **Automated Cleanup**: Relocates `.adb` files to build directory
- **Cross-Platform Path Handling**: Manages Windows path separators and spaces
- **Build Verification**: Executes `ihxcheck` to detect linker errors

The Makefile includes detailed inline documentation for reference.

## License

See the license file in the project root.
