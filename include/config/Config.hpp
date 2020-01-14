#ifndef SINGLETON_CONFIG_H
#define SINGLETON_CONFIG_H

#include "config/SettingsJsonParser.hpp"

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class Config {
public:
    static SettingsJsonParser& getInstance();
    static void setProjectPath(const std::string& project_path);
    static std::string getProjectPath();
    static std::string getAbsolutePath(const std::string& path);

private:
    Config() = default;
    ~Config() = default;
    Config(const Config&)= delete;
    Config& operator=(const Config&)= delete;

    static std::string project_path_;
};

int load_config();
fs::path get_selfpath();
fs::path get_project_path();

#endif /* SINGLETON_CONFIG_H */

