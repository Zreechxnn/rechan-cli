#include "command_dispatcher.hpp"
#include "color.hpp"
#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#define APP_NAME "ReChan CLI"
#define APP_VERSION "2.0.0"

// Function to show a GUI popup
void showPopup(const QString& title, const QString& message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.exec();
}

void displayHelp() {
    // Header dengan warna
    std::cout << Color::RED << Color::BOLD
              << " _ __ ___  ___| |__    __ _ _ __  \n"
              << "| '__/ _ \\/ __| '_ \\ / _` | '_ \\ \n"
              << "| | |  __/ (__| | | | (_| | | | |\n"
              << "|_|  \\___|\\___|_| |_|\\__,_|_| |_|\n"
              << "ganteng\n"
              << Color::RESET
              << "Version " << APP_VERSION << "\n\n";

    std::cout << Color::YELLOW << "Usage: rechan <command> [args]\n\n"
              << Color::RESET;

    // Tabel commands
    std::cout << Color::GREEN << "+-------------------------+---------------------------------------------------------------+----------------------+" << Color::RESET << std::endl;
    std::cout << Color::GREEN << "| " << Color::CYAN << std::left << std::setw(23) << "Command"
              << Color::GREEN << " | " << Color::CYAN << std::setw(59) << "Description"
              << Color::GREEN << " | " << Color::CYAN << std::setw(20) << "Options"
              << Color::GREEN << " |" << Color::RESET << std::endl;
    std::cout << Color::GREEN << "+-------------------------+---------------------------------------------------------------+----------------------+" << Color::RESET << std::endl;

    // Baris-baris data
    std::cout << Color::GREEN << "| " << Color::WHITE << std::setw(23) << "create <type> <name>"
              << Color::GREEN << " | " << Color::WHITE << std::setw(59) << "Creates a new project from template"
              << Color::GREEN << " | " << Color::YELLOW << std::setw(20) << "--version <v>"
              << Color::GREEN << " |" << Color::RESET << std::endl;

    std::cout << Color::GREEN << "| " << Color::WHITE << std::setw(23) << ""
              << Color::GREEN << " | " << Color::WHITE << std::setw(59) << "Types: laravel, react, vue, cpp, etc."
              << Color::GREEN << " | " << Color::YELLOW << std::setw(20) << ""
              << Color::GREEN << " |" << Color::RESET << std::endl;

    std::cout << Color::GREEN << "| " << Color::WHITE << std::setw(23) << "compile [dir]"
              << Color::GREEN << " | " << Color::WHITE << std::setw(59) << "Compiles project in specified directory"
              << Color::GREEN << " | " << Color::YELLOW << std::setw(20) << ""
              << Color::GREEN << " |" << Color::RESET << std::endl;

    std::cout << Color::GREEN << "| " << Color::WHITE << std::setw(23) << "serve"
              << Color::GREEN << " | " << Color::WHITE << std::setw(59) << "Runs development server in current directory"
              << Color::GREEN << " | " << Color::YELLOW << std::setw(20) << "--port <p>"
              << Color::GREEN << " |" << Color::RESET << std::endl;

    std::cout << Color::GREEN << "| " << Color::WHITE << std::setw(23) << "run [dir]"
              << Color::GREEN << " | " << Color::WHITE << std::setw(59) << "Runs project in specified directory"
              << Color::GREEN << " | " << Color::YELLOW << std::setw(20) << ""
              << Color::GREEN << " |" << Color::RESET << std::endl;

    std::cout << Color::GREEN << "| " << Color::WHITE << std::setw(23) << "--help"
              << Color::GREEN << " | " << Color::WHITE << std::setw(59) << "Shows this help message"
              << Color::GREEN << " | " << Color::YELLOW << std::setw(20) << ""
              << Color::GREEN << " |" << Color::RESET << std::endl;

    std::cout << Color::GREEN << "| " << Color::WHITE << std::setw(23) << "--version, -v"
              << Color::GREEN << " | " << Color::WHITE << std::setw(59) << "Shows application version"
              << Color::GREEN << " | " << Color::YELLOW << std::setw(20) << ""
              << Color::GREEN << " |" << Color::RESET << std::endl;

    std::cout << Color::GREEN << "+-------------------------+---------------------------------------------------------------+----------------------+" << Color::RESET << std::endl;

    // Contoh penggunaan
    std::cout << Color::YELLOW << "\nExamples:" << Color::RESET << std::endl;
    std::cout << "  " << Color::WHITE << "rechan create laravel myapp --version 10" << Color::RESET << std::endl;
    std::cout << "  " << Color::WHITE << "rechan serve --port 3000" << Color::RESET << std::endl;
    std::cout << "  " << Color::WHITE << "rechan compile ./myproject" << Color::RESET << std::endl;
}

void displayVersion()
{
    std::cout << Color::CYAN << Color::BOLD << APP_NAME << Color::RESET
              << " " << APP_VERSION << std::endl;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc < 2)
    {
        displayHelp();
        return 0;
    }

    std::vector<std::string> args(argv + 1, argv + argc);
    std::string command = args[0];
    CommandDispatcher dispatcher;

    try
    {
        if (command == "--help")
        {
            displayHelp();
        }
        else if (command == "--version" || command == "-v")
        {
            displayVersion();
        }
        else if (command == "create")
        {
            if (args.size() < 3)
                throw std::runtime_error("Usage: rechan create <type> <name> [options]");
            std::string type = args[1];
            std::string name = args[2];
            std::string version = "";
            if (args.size() > 3 && args[3] == "--version" && args.size() > 4)
            {
                version = args[4];
            }
            dispatcher.create(type, name, version);
            showPopup("Success", "Project '" + QString::fromStdString(name) + "' created successfully!");
        }
        else if (command == "compile")
        {
            fs::path dir = (args.size() > 1) ? args[1] : ".";
            dispatcher.compile(dir);
            showPopup("Success", "Project compiled successfully!");
        }
        else if (command == "serve")
        {
            int port = 8000;
            if (args.size() > 1 && args[1] == "--port" && args.size() > 2)
            {
                port = std::stoi(args[2]);
            }
            dispatcher.serve(".", port); // serve always runs in current dir
        }
        else if (command == "run")
        {
            fs::path dir = (args.size() > 1) ? args[1] : ".";
            dispatcher.run(dir);
        }
        else
        {
            throw std::runtime_error("Unknown command: " + command);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << Color::RED << "Error: " << e.what() << Color::RESET << std::endl;
        showPopup("Error", e.what());
        return 1;
    }

    return 0;
}