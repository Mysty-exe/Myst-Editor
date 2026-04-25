# Terminal Text Editor (Myst)

A terminal based text editor built with C++ and ncurses.  
The editor focuses on fast keyboard driven editing, customizable themes, and lightweight performance in terminal environments.

## Overview

Myst is designed for efficient text editing directly in the terminal.  
It handles user input, rendering, and file operations without relying on external editors, with a focus on responsiveness and control.

## Core Features

- Keyboard driven editing workflow
- Syntax highlighting for Python
- Theme system with configurable color schemes
- Multi file navigation and switching
- Save and Save As functionality
- Configurable user preferences
- Lightweight and fast execution

## Demo

## Technical Details

### Input System

- Uses ncurses for raw keyboard input handling
- Processes key events for navigation and editing commands
- Uses custom keybindings and shortcuts

### Text Buffer

- Manages text using a structured in memory buffer
- Handles insertion, deletion, and navigation efficiently
- Maintains cursor position and viewport state

### Rendering

- Uses ncurses for terminal rendering
- Redraws only necessary regions for performance
- Applies syntax highlighting during render pass

### Syntax Highlighting

- Token based parsing for Python files
- Identifies keywords, strings, and comments
- Applies color mapping based on theme configuration

### Configuration

- Reads user preferences from configuration files
- Supports theme switching and editor behavior customization
- Applies settings at startup

## Requirements

- C++17 compatible compiler
- ncurses
- make
- xclip

## Build

Clone the repository:

```
git clone https://github.com/Mysty-exe/Myst.git
cd Myst
```

Build and run:

```
make all
```

## Run

```
./build/myst.exe
```

Open a file:

```
./build/myst.exe filename.txt
```

## Controls

General

- Ctrl Q: quit
- Ctrl S: save
- Ctrl E: open menu
- Ctrl P: preferences

Navigation

- Ctrl B: previous file
- Ctrl N: next file

Editing

- Ctrl F: find and replace

## Project Structure

```
src/ core    editor logic
include/     headers
config/      user settings
resources/   themes and assets
build/       generated files
```

## Future Work

- Support for more programming languages
- Improved syntax parsing system
- More efficient text buffer structure
- Plugin system for extensions
- Mouse support

## Notes

- Designed for Linux terminal environments
- Requires proper ncurses support
