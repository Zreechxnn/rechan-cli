#include "command_dispatcher.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstdlib>

// Helper to replace all occurrences of a substring
void CommandDispatcher::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

// Executes a system command, optionally in a specific working directory
void CommandDispatcher::executeCommand(const std::string& command, const fs::path& workingDir) {
    std::string finalCommand;
    if (!workingDir.empty()) {
        // Change directory command differs between OS
#ifdef _WIN32
        finalCommand = "cd /d \"" + workingDir.string() + "\" && " + command;
#else
        finalCommand = "cd \"" + workingDir.string() + "\" && " + command;
#endif
    } else {
        finalCommand = command;
    }

    std::cout << "Executing: " << finalCommand << std::endl;
    int result = std::system(finalCommand.c_str());
    if (result != 0) {
        throw std::runtime_error("Command failed with exit code " + std::to_string(result) + ": " + command);
    }
}

// Loads configuration from commands.json with a fallback mechanism
json CommandDispatcher::loadConfig() {
    fs::path exePath = "."; // A sensible default
    // This part is tricky without Qt's application path. We'll assume it's findable.
    // For a real app, you'd use a more robust way to find the executable path.
    // Let's check current dir, then maybe a system-wide location.
    
    fs::path configName = "commands.json";
    fs::path localPath = configName;
    // fs::path exePath = QCoreApplication::applicationDirPath(); // If using Qt
    // fs::path globalPath = "/etc/rechan/commands.json"; // Linux example

    if (fs::exists(localPath)) {
        std::ifstream f(localPath);
        return json::parse(f);
    }

    // Add more fallbacks here if needed (e.g., next to executable, user home dir)
    throw std::runtime_error("Failed to load commands.json. Make sure it's in the current directory.");
}

// Creates a new project based on templates
void CommandDispatcher::create(const std::string& type, const std::string& name, const std::string& version) {
    json config = loadConfig();
    std::string actualType = type;
    std::string actualVersion = version;

    size_t colonPos = type.find(':');
    if (colonPos != std::string::npos) {
        actualType = type.substr(0, colonPos);
    }
    
    if (!config["templates"].contains(actualType)) {
        throw std::runtime_error("Template not found for type: " + actualType);
    }
    
    auto& tpl = config["templates"][actualType];

    if (tpl.contains("create_command")) {
        std::string command = tpl["create_command"].get<std::string>();
        std::string versionSpec = actualVersion.empty() ? "" : "@" + actualVersion;
        replaceAll(command, "{{name}}", name);
        replaceAll(command, "{{version}}", versionSpec);
        
        // Handle special case for Drogon controller
        if (type.find("drogon:controller") != std::string::npos) {
             std::string controllerName = type.substr(colonPos + 1);
             replaceAll(command, "{{name}}", controllerName);
        }

        executeCommand(command);

        if (tpl.contains("post_create")) {
            for (const auto& cmd : tpl["post_create"]) {
                std::string postCmd = cmd.get<std::string>();
                replaceAll(postCmd, "{{name}}", name);
                executeCommand(postCmd);
            }
        }
    } else if (tpl.contains("files")) {
        fs::path projectDir(name);
        fs::create_directory(projectDir);
        for (auto& [filename, content] : tpl["files"].items()) {
            fs::path filePath = projectDir / filename;
            fs::create_directories(filePath.parent_path());
            
            std::ofstream file(filePath);
            std::string fileContent = content.get<std::string>();
            replaceAll(fileContent, "{{name}}", name);
            file << fileContent;
        }
    }
}

// Detects the project type based on files present in the directory
std::string CommandDispatcher::detectProjectType(const fs::path& dir) {
    if (fs::exists(dir / "composer.json")) return "laravel";
    if (fs::exists(dir / "drogon.pro")) return "drogon";
    if (fs::exists(dir / "package.json")) {
        // Could be more specific by reading package.json, but this is a good start
        return "nodejs";
    }
    if (fs::exists(dir / "CMakeLists.txt")) {
        std::ifstream file(dir / "CMakeLists.txt");
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if (content.find("SDL2") != std::string::npos) return "sdl";
        return "cpp";
    }
    if (fs::exists(dir / "main.py")) return "python";
    if (fs::exists(dir / "index.php")) return "php";
    
    return "unknown";
}

// Compiles a project
void CommandDispatcher::compile(const fs::path& projectDir) {
    json config = loadConfig();
    std::string projectType = detectProjectType(projectDir);
    
    if (!config["commands"]["compile"].contains(projectType)) {
        throw std::runtime_error("No compile command found for project type: " + projectType);
    }
    
    std::string command = config["commands"]["compile"][projectType].get<std::string>();
    replaceAll(command, "{{name}}", projectDir.filename().string());
    executeCommand(command, projectDir);
}

// Serves a project
void CommandDispatcher::serve(const fs::path& projectDir, int port) {
    json config = loadConfig();
    std::string projectType = detectProjectType(projectDir);

    if (projectType == "unknown") {
        throw std::runtime_error("Could not detect project type in directory: " + projectDir.string());
    }
    if (!config["commands"]["serve"].contains(projectType)) {
        throw std::runtime_error("No serve command found for project type: " + projectType);
    }

    std::string command = config["commands"]["serve"][projectType].get<std::string>();
    replaceAll(command, "{{port}}", std::to_string(port));
    replaceAll(command, "{{name}}", projectDir.filename().string());

    executeCommand(command, projectDir);
}

// Runs a project
void CommandDispatcher::run(const fs::path& projectDir) {
    json config = loadConfig();
    std::string projectType = detectProjectType(projectDir);

    // Some project types need compilation before running
    if (projectType == "cpp" || projectType == "sdl" || projectType == "drogon") {
        std::cout << "Compiling project first..." << std::endl;
        compile(projectDir);
    }

    if (projectType == "unknown") {
        throw std::runtime_error("Could not detect project type in directory: " + projectDir.string());
    }
    if (!config["commands"]["run"].contains(projectType)) {
        throw std::runtime_error("No run command found for project type: " + projectType);
    }

    std::string command = config["commands"]["run"][projectType].get<std::string>();
    replaceAll(command, "{{name}}", projectDir.filename().string());
    
    executeCommand(command, projectDir);
}