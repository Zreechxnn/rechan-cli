#include "command_dispatcher.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QFileInfo>
#include <sstream>

using json = nlohmann::json;

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
}

void CommandDispatcher::executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        throw std::runtime_error("Command failed: " + command);
    }
}

json CommandDispatcher::loadConfig() {
    // 1. Try to load from Qt resources
    QFile resourceFile(":/commands.json");
    if (resourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&resourceFile);
        std::string jsonData = in.readAll().toStdString();
        resourceFile.close();
        try {
            return json::parse(jsonData);
        } catch (const json::exception& e) {
            // Continue to next fallback
        }
    }

    // 2. Try executable directory
    QString exePath = QCoreApplication::applicationDirPath();
    QFile exeFile(exePath + "/commands.json");
    if (exeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&exeFile);
        std::string jsonData = in.readAll().toStdString();
        exeFile.close();
        try {
            return json::parse(jsonData);
        } catch (const json::exception& e) {
            // Continue to next fallback
        }
    }

    // 3. Try current working directory
    std::ifstream localFile("commands.json");
    if (localFile.is_open()) {
        try {
            return json::parse(localFile);
        } catch (const json::exception& e) {
            // Continue to throw error
        }
    }

    throw std::runtime_error("Failed to load commands.json from any location");
}

void CommandDispatcher::create(const std::string& type, const std::string& name, const std::string& version) {
    json config = loadConfig();

    // Ekstrak versi dari type jika menggunakan format "framework:versi"
    std::string actualType = type;
    std::string actualVersion = version;
    
    size_t colonPos = type.find(':');
    if (colonPos != std::string::npos) {
        actualType = type.substr(0, colonPos);
        actualVersion = type.substr(colonPos + 1);
    }

    // Jika versi diberikan melalui parameter, utamakan parameter
    if (!version.empty()) {
        actualVersion = version;
    }

    if (!config.contains("templates") || !config["templates"].contains(actualType)) {
        throw std::runtime_error("Template not found for type: " + actualType);
    }

    auto& templateConfig = config["templates"][actualType];

    // Handle templates with create_command
    if (templateConfig.contains("create_command")) {
        std::string command = templateConfig["create_command"].get<std::string>();
        
        // Format versi untuk package manager
        std::string versionSpec = actualVersion.empty() ? "" : "@" + actualVersion;
        replaceAll(command, "{{name}}", name);
        replaceAll(command, "{{version}}", versionSpec);
        
        executeCommand(command);

        // Jalankan post_create commands jika ada
        if (templateConfig.contains("post_create")) {
            for (auto& cmd : templateConfig["post_create"]) {
                std::string postCmd = cmd.get<std::string>();
                replaceAll(postCmd, "{{name}}", name);
                executeCommand(postCmd);
            }
        }
        return;
    }

    // Default template-based creation
    QDir projectDir(QString::fromStdString(name));
    if (!projectDir.mkpath(".")) {
        throw std::runtime_error("Failed to create project directory: " + name);
    }

    auto& files = templateConfig["files"];
    for (auto& [filename, content] : files.items()) {
        QString filePath = projectDir.filePath(QString::fromStdString(filename));
        QFileInfo fileInfo(filePath);
        QDir().mkpath(fileInfo.absolutePath());

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw std::runtime_error("Failed to create file: " + filePath.toStdString());
        }

        QTextStream out(&file);
        std::string fileContent = content.get<std::string>();
        replaceAll(fileContent, "{{name}}", name);
        out << QString::fromStdString(fileContent);
        file.close();
    }
}

std::string CommandDispatcher::detectProjectType(const std::string& dir) {
    QDir projectDir(QString::fromStdString(dir));

    if (projectDir.exists("composer.json")) return "laravel";
    if (projectDir.exists("package.json")) return "nodejs";

    if (projectDir.exists("CMakeLists.txt")) {
        QFile cmakeFile(projectDir.filePath("CMakeLists.txt"));
        if (cmakeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&cmakeFile);
            QString content = in.readAll();
            if (content.contains("SDL2")) return "sdl";
            return "cpp";
        }
    }

    if (projectDir.exists("index.php")) return "php";

    return "unknown";
}

void CommandDispatcher::compile(const std::string& projectDir) {
    json config = loadConfig();
    std::string projectType = detectProjectType(projectDir);

    std::string command;
    if (config["commands"]["compile"].contains(projectType)) {
        command = config["commands"]["compile"][projectType].get<std::string>();
    } else if (config["commands"]["compile"].contains("default")) {
        command = config["commands"]["compile"]["default"].get<std::string>();
    } else {
        throw std::runtime_error("No compile command found for project type: " + projectType);
    }

    replaceAll(command, "{{name}}", projectDir);
    executeCommand("cd \"" + projectDir + "\" && " + command);
}

void CommandDispatcher::serve(const std::string& type, int port) {
    json config = loadConfig();

    if (!config["commands"]["serve"].contains(type)) {
        throw std::runtime_error("Unsupported serve type: " + type);
    }

    std::string command = config["commands"]["serve"][type].get<std::string>();
    replaceAll(command, "{{port}}", std::to_string(port));
    
    // Get project name from current directory
    QDir currentDir(QDir::currentPath());
    std::string projectName = currentDir.dirName().toStdString();
    replaceAll(command, "{{name}}", projectName);

    executeCommand(command);
}

void CommandDispatcher::init(const std::string& type, const std::string& name) {
    json config = loadConfig();

    if (type == "controller") {
        if (!config.contains("drogon_ctl") || 
            !config["drogon_ctl"].contains("create_controller")) {
            throw std::runtime_error("drogon_ctl command not configured");
        }

        std::string command = config["drogon_ctl"]["create_controller"].get<std::string>();
        replaceAll(command, "{{name}}", name);
        executeCommand(command);
    } else {
        throw std::runtime_error("Unsupported init type: " + type);
    }
}