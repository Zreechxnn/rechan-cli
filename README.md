# Rechan CLI

![Rechan CLI Logo](https://raw.githubusercontent.com/Zreechxnn/rechan-cli/main/resources/icon.ico)

**Rechan CLI** is a powerful command-line tool designed to streamline your development workflow. It simplifies project creation, compilation, and execution for multiple languages and frameworks.

## Features

- 🚀 **Multi-language Support**: Create projects in C++, Python, PHP, Laravel, and Drogon
- ⚡️ **Smart Compilation**: Automatic project detection and compilation
- 🌐 **Built-in Web Server**: Run development servers with one command
- 🔧 **Template System**: Pre-configured templates for various project types
- 📦 **Dependency Management**: Integrated package handling

## Installation

### Prerequisites
- Qt5 (minimum 5.15)
- C++17 compatible compiler
- CMake (minimum 3.15)
- Python 3.x (for Python projects)

### Build from Source
```bash
git clone https://github.com/yourusername/rechan-cli.git
cd rechan-cli
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Windows Installer
Download the latest installer from our [Releases page](https://github.com/yourusername/rechan-cli/releases)

## Quick Start

### Create a New Project
```bash
rechan create python my_python_app
rechan create drogon my_web_api
rechan create controller UserController --project my_web_api
```

### Compile Your Project
```bash
rechan compile my_web_api
```

### Run a Development Server
```bash
rechan serve python  # Runs Python project
rechan serve drogon  # Runs Drogon project on default port
```

## Command Reference

### `rechan create <type> <name> [options]`
Creates a new project of specified type

| Type        | Description                  | Options                     |
|-------------|------------------------------|-----------------------------|
| `python`    | Python application           |                             |
| `drogon`    | Drogon C++ web framework     |                             |
| `controller`| Drogon controller            | `--project` (required)      |
| `laravel`   | Laravel PHP framework        | `--laravel-version`         |
| `cpp`       | Basic C++ project            |                             |
| `sdl`       | SDL2 multimedia project      |                             |
| `php`       | Simple PHP project           |                             |

### `rechan compile [directory]`
Compiles the project in the specified directory (current directory by default)

### `rechan serve <type> <port>`
Runs a development server for the specified project type

### `rechan --version`
Shows CLI version information

### `rechan --help`
Displays help information and command usage

## Project Templates

Rechan CLI comes with pre-configured templates for various project types:

### Python Template
```python
print("Hello, Rechan Python!")
```

### Drogon Template (CMakeLists.txt)
```cmake
cmake_minimum_required(VERSION 3.15)
project(my_project)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
```

### SDL Template (main.cpp)
```cpp
#include <SDL.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Rechan SDL", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        800, 600, 0);
    
    // Your game loop here
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
```

## Configuration

Rechan CLI uses a `commands.json` file for configuration. The CLI searches for this file in:
1. Embedded application resources
2. Executable directory
3. Current working directory

Example configuration:
```json
{
  "templates": {
    "python": {
      "files": {
        "main.py": "print(\"Hello, Rechan Python!\")",
        "requirements.txt": ""
      }
    }
  },
  "commands": {
    "compile": {
      "python": "",
      "drogon": "cmake -B build && cmake --build build"
    }
  }
}
```

## Contributing

We welcome contributions! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/awesome-feature`)
3. Commit your changes (`git commit -am 'Add awesome feature'`)
4. Push to the branch (`git push origin feature/awesome-feature`)
5. Open a pull request

## License

Rechan CLI is licensed under the [MIT License](LICENSE).

---

**Rechan CLI** - Accelerate your development workflow!  
Version 1.2.0 | [Documentation](docs) | [Report Issue](issues)
