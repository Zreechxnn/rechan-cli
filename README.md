# Rechan CLI

![Qt-based CLI Tool](https://img.shields.io/badge/Qt-6.7.0-green)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Version](https://img.shields.io/badge/version-1.2.0-orange)

Rechan CLI is a Qt-based command-line tool for creating, compiling, and managing various programming projects with easy configuration.

## Key Features

- 🛠️ **Create new projects** from ready-to-use templates:
  - C++
  - SDL2
  - PHP
  - Python
  - Laravel
  - Drogon
- ⚙️ **Automatic compilation** with project-specific configurations
- 🚀 **Run development servers** for various frameworks
- 📦 **Template management** through centralized JSON configuration
- 💡 **Graphical UI popups** for notifications

## Installation

### Prerequisites
- [CMake](https://cmake.org/) (v3.15+)
- [Qt](https://www.qt.io/) (v6.7.0)
- [vcpkg](https://vcpkg.io/) (for dependency management)
- C++ compiler (MSVC/GCC/Clang)

### Build Steps
```bash
git clone https://github.com/username/rechan-cli.git
cd rechan-cli
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" \
         -DCMAKE_PREFIX_PATH="path/to/Qt/6.7.0/msvc2019_64"
cmake --build . --config Release
```

## Usage

```
Rechan CLI 1.2.0
Usage: rechan <command> [args]

Available commands:
  create <type> <name> [options]
    types: laravel, drogon, controller, cpp, sdl, php, python
    options:
      --laravel-version <version>  Specify Laravel version
      --project <path>             Project path (for Drogon controllers)

  compile [dir]       Compile project
  init <type> <name>  Initialize project components
  serve <type> <port> Start development server
  --version           Show version
```

### Command Examples

**Create SDL2 project:**
```bash
rechan create sdl my_sdl_game
```

**Compile C++ project:**
```bash
rechan compile ./my_cpp_project
```

**Run PHP server:**
```bash
rechan serve php 8080
```

**Create Drogon controller:**
```bash
rechan init controller MyController
```

**Create Laravel project:**
```bash
rechan create laravel my_app --laravel-version=10.*
```

## Project Structure

```
rechan-cli/
├── CMakeLists.txt
├── build.bat
├── main.cpp
├── command_dispatcher.hpp
├── command_dispatcher.cpp
├── commands.json
└── resources/
    └── resources.qrc
```

## Template Configuration

Project templates are configured in `commands.json`:

```json
{
  "templates": {
    "cpp": {
      "files": {
        "CMakeLists.txt": "...",
        "main.cpp": "..."
      }
    },
    "sdl": {
      "files": {
        "CMakeLists.txt": "...",
        "main.cpp": "..."
      }
    }
  },
  "commands": {
    "compile": {
      "default": "cmake -B build && cmake --build build",
      "sdl": "cmake -B build -DCMAKE_TOOLCHAIN_FILE=...",
      "laravel": "cd {{name}} && composer install && npm install"
    },
    "serve": {
      "php": "php -S localhost:{{port}}",
      "laravel": "php artisan serve --port={{port}}"
    }
  }
}
```

## Project Support Matrix

| Project Type | Create | Compile | Serve |
|-------------|------|-----------|-------|
| C++         | ✅   | ✅        | ❌    |
| SDL2        | ✅   | ✅        | ❌    |
| PHP         | ✅   | ❌        | ✅    |
| Python      | ✅   | ❌        | ✅    |
| Laravel     | ✅   | ✅        | ✅    |
| Drogon      | ✅   | ✅        | ✅    |

## Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b new-feature`)
3. Commit changes (`git commit -am 'Add new feature'`)
4. Push to branch (`git push origin new-feature`)
5. Create Pull Request

## License

This project is licensed under the [MIT License](LICENSE).

---

Built with ❤️ using Qt and C++
