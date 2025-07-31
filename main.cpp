#include <iostream>
#include <string>
#include <QApplication>
#include <QMessageBox>
#include <QIcon>
#include "command_dispatcher.hpp"

#define APP_VERSION "1.2.0"

void showPopup(const QString& title, const QString& message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.exec();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.ico"));

    if (argc < 2 || std::string(argv[1]) == "--help") {
        std::cout << "Rechan CLI " << APP_VERSION << "\n";
        std::cout << "Usage: rechan <command> [args]\n\n";
        std::cout << "Available commands:\n";
        std::cout << "  create <type> <name> [options]\n";
        std::cout << "    types: laravel, drogon, controller, cpp, sdl, php, python\n";
        std::cout << "    options:\n";
        std::cout << "      --laravel-version <version>  Specify Laravel version\n";
        std::cout << "      --project <path>             Project path (for Drogon controllers)\n\n";
        std::cout << "  compile [dir]       Compile project\n";
        std::cout << "  init <type> <name>  Initialize project\n";
        std::cout << "  serve <type> <port> Start development server\n";
        std::cout << "  --version           Show version\n";
        return 0;
    }

    CommandDispatcher dispatcher;
    std::string command = argv[1];

    try {
        if (command == "--version" || command == "-v") {
            std::cout << "Rechan CLI version " << APP_VERSION << std::endl;
        }
        // In the create command section:
        else if (command == "create") {
            if (argc < 4) throw std::runtime_error("Syntax: rechan create <type> <name> [options]");

            std::string version = "";
            for (int i = 4; i < argc; i++) {
                if (std::string(argv[i]) == "--laravel-version" && i + 1 < argc) {
                    version = argv[i + 1];
                    i++;  // Skip version value
                }
            }

            try {
                dispatcher.create(argv[2], argv[3], version);
                showPopup("Success", "Project created successfully!");
            } catch (const std::exception& e) {
                std::string errorMsg = e.what();
                if (errorMsg.find("php") != std::string::npos) {
                    errorMsg += "\n\nMake sure PHP is installed and added to your PATH";
                }
                throw std::runtime_error(errorMsg);
            }
        }
        else if (command == "compile") {
            std::string projectDir = (argc >= 3) ? argv[2] : ".";
            dispatcher.compile(projectDir);
        }
        else if (command == "init") {
            if (argc < 4) throw std::runtime_error("Syntax: rechan init <type> <name>");
            dispatcher.init(argv[2], argv[3]);
        }
        else if (command == "serve") {
            if (argc < 4) throw std::runtime_error("Syntax: rechan serve <type> <port>");
            dispatcher.serve(argv[2], std::stoi(argv[3]));
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