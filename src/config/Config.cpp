#include "config/Config.hpp"

#include <unistd.h>

inline std::string conf_path = "config.json";

std::string Config::project_path_;

SettingsJsonParser& Config::getInstance() {
	static SettingsJsonParser instance;
	return instance;
}

void Config::setProjectPath(const std::string& project_path) {
    project_path_ = project_path;
}

std::string Config::getProjectPath() {
    return project_path_;
}

std::string Config::getAbsolutePath(const std::string& path){
    return project_path_ + "/" + path;
}

int load_config() {
    if (!Config::getInstance().loadFromFile(Config::getAbsolutePath(conf_path))) {
        fprintf(stderr, "Error loading config file\n");
        return -1;
    }
    return 0;
}


fs::path get_selfpath() {
    char buff[255];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);

    if (len != -1) {
        buff[len] = '\0';
        return fs::path(buff);
    }

    return fs::path();
}

fs::path get_project_path() {
    return get_selfpath().parent_path().parent_path();
}
