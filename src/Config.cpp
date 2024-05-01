#include "Config.hpp"
#include <iostream>
#include <cctype>

Config::Config(const std::string& filename) {
    std::ifstream myFile(filename);
    if (!myFile.is_open()) {
        std::cerr << "Could not open config file, proceeding with default values.." << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (getline(myFile, line)) {
        std::string trimmed = trim(line);
        if (!trimmed.empty() && trimmed[0] != '#') {
            lines.push_back(trimmed);
        }
    }

    // Extract configuration values
    port = stoi(initValue("port", lines));
    path_to_file = initValue("logpath", lines);
    rconPass = initValue("rcon", lines);
    ip = initValue("ip", lines);
}

// Utility method to initialize values from config lines
std::string Config::initValue(const std::string& query, const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = trim(line.substr(0, delimiterPos));
            if (key == query) {
                return trim(line.substr(delimiterPos + 1));
            }
        }
    }
    return {};
}

// Utility method to trim whitespace from a string
std::string Config::trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();

    return (start < end ? std::string(start, end) : std::string());
}

// Getter methods
std::string Config::getPathToFile() const {
    return path_to_file;
}

int Config::getPort() const {
    return port;
}

std::string Config::getIP() const {
    return ip;
}

std::string Config::getRconPass() const {
    return rconPass;
}
