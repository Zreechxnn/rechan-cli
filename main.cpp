#include <iostream>
#include <string>
#include <vector>
#include <QApplication>
#include <QMessageBox>
#include <QIcon>
#include "command_dispatcher.hpp"

#define APP_NAME "ReChan"
#define APP_VERSION "1.3.0"

// Deklarasi fungsi pembantu
void showPopup(const QString& title, const QString& message);
void displayHelp();
void displayVersion();
void handleCreateCommand(CommandDispatcher& dispatcher, int argc, char* argv[]);
void handleCompileCommand(CommandDispatcher& dispatcher, int argc, char* argv[]);
void handleInitCommand(CommandDispatcher& dispatcher, int argc, char* argv[]);
void handleServeCommand(CommandDispatcher& dispatcher, int argc, char* argv[]);

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.ico"));

    if (argc < 2 || std::string(argv[1]) == "--help") {
        displayHelp();
        return 0;
    }

    CommandDispatcher dispatcher;
    std::string command = argv[1];

    try {
        if (command == "--version" || command == "-v") {
            displayVersion();
        }
        else if (command == "create") {
            handleCreateCommand(dispatcher, argc, argv);
        }
        else if (command == "compile") {
            handleCompileCommand(dispatcher, argc, argv);
        }
        else if (command == "init") {
            handleInitCommand(dispatcher, argc, argv);
        }
        else if (command == "serve") {
            handleServeCommand(dispatcher, argc, argv);
        }
        else {
            throw std::runtime_error("Unknown command: " + command);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        showPopup("Error", e.what());
        return 1;
    }

    return 0;
}

// Implementasi fungsi pembantu
void showPopup(const QString& title, const QString& message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.exec();
}

void displayHelp() {
    std::cout <<
        " _ __ ___  ___| |__   __ _ _ __  \n"
        "| '__/ _ \\/ __| '_ \\ / _` | '_ \\ \n"
        "| | |  __/ (__| | | | (_| | | | |\n"
        "|_|  \\___|\\___|_| |_|\\__,_|_| |_|\n"
        << APP_VERSION << "\n\n";

    std::cout << "Usage: rechan <command> [args]\n\n";
    std::cout << "Available commands:\n";
    std::cout << "  create <type> <name> [options]\n";
    std::cout << "    types: laravel, drogon, controller, cpp, sdl, php, python, "
              << "react, vue, angular, svelte, next, nuxt, express, nest, astro, remix, solid, qwik\n";
    std::cout << "    options:\n";
    std::cout << "      --version <version>  Specify framework version\n";
    std::cout << "      --project <path>     Project path (for Drogon controllers)\n\n";
    std::cout << "  compile [dir]       Compile project\n";
    std::cout << "  init <type> <name>  Initialize project\n";
    std::cout << "  serve <type> <port> Start development server\n";
    std::cout << "  --version           Show version\n";
}

void displayVersion() {
    std::cout << APP_NAME << std::endl << APP_VERSION << std::endl;
}

void handleCreateCommand(CommandDispatcher& dispatcher, int argc, char* argv[]) {
    if (argc < 4) {
        throw std::runtime_error("Syntax: rechan create <type> <name> [options]");
    }

    std::string version = "";
    std::string projectPath = "";

    // Parse options
    for (int i = 4; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--version" && (i + 1) < argc) {
            version = argv[i + 1];
            i++;  // Skip next argument
        }
        else if (arg == "--project" && (i + 1) < argc) {
            projectPath = argv[i + 1];
            i++;  // Skip next argument
        }
    }

    try {
        // Jika ada projectPath, tambahkan ke dispatcher (asumsi dispatcher mendukung)
        if (!projectPath.empty()) {
            // Implementasi spesifik untuk projectPath
        }
        
        dispatcher.create(argv[2], argv[3], version);
        showPopup("Success", "Project created successfully!");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        
        // Tambahkan saran troubleshooting berdasarkan error
        if (errorMsg.find("npm") != std::string::npos || 
            errorMsg.find("npx") != std::string::npos) {
            errorMsg += "\n\nMake sure Node.js and npm are installed and in your PATH";
        } 
        else if (errorMsg.find("php") != std::string::npos) {
            errorMsg += "\n\nMake sure PHP is installed and in your PATH";
        } 
        else if (errorMsg.find("composer") != std::string::npos) {
            errorMsg += "\n\nMake sure Composer is installed and in your PATH";
        }
        
        throw std::runtime_error(errorMsg);
    }
}

void handleCompileCommand(CommandDispatcher& dispatcher, int argc, char* argv[]) {
    std::string projectDir = (argc >= 3) ? argv[2] : ".";
    dispatcher.compile(projectDir);
}

void handleInitCommand(CommandDispatcher& dispatcher, int argc, char* argv[]) {
    if (argc < 4) {
        throw std::runtime_error("Syntax: rechan init <type> <name>");
    }
    dispatcher.init(argv[2], argv[3]);
}

void handleServeCommand(CommandDispatcher& dispatcher, int argc, char* argv[]) {
    if (argc < 4) {
        throw std::runtime_error("Syntax: rechan serve <type> <port>");
    }
    
    try {
        int port = std::stoi(argv[3]);
        dispatcher.serve(argv[2], port);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Invalid port number: " + std::string(argv[3]));
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Port number out of range: " + std::string(argv[3]));
    }
}