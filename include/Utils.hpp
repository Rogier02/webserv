#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <map>

namespace Utils {
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string getExtension(const std::string& path);
    std::string urlDecode(const std::string& str);
    std::string getStatusMessage(int statusCode);
    std::string getTimestamp();
    bool endsWith(const std::string& str, const std::string& suffix);
    std::string intToString(int value);
}

#endif
