#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

class CommandDispatcher {
public:
    void create(const std::string& type, const std::string& name, const std::string& version = "");
    void compile(const fs::path &projectDir = ".");
    void serve(const fs::path &projectDir = ".", int port = 8000);
    void run(const fs::path &projectDir = ".");

private:
    void executeCommand(const std::string &command, const fs::path &workingDir = "");
    json loadConfig();
    std::string detectProjectType(const fs::path &dir);
    void replaceAll(std::string &str, const std::string &from, const std::string &to);
};

#endif // COMMAND_DISPATCHER_HPP