#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include <string>
#include <nlohmann/json.hpp>

class CommandDispatcher {
public:
    void create(const std::string& type, const std::string& name, const std::string& version = "");
    void compile(const std::string& projectDir = ".");
    void init(const std::string& type, const std::string& name);
    void serve(const std::string& type, int port);

private:
    void executeCommand(const std::string& command);
    nlohmann::json loadConfig();
    std::string detectProjectType(const std::string& dir);
};

#endif