#pragma once

#include <string>
#include <vector>

class Configuration {
public:
    std::vector<std::string> all_components;
    std::vector<std::string> pre_components;
    std::vector<std::string> gaze_components;
    std::vector<std::string> post_components;
    
    Configuration() {};
    ~Configuration() {};
    
    void load(const std::string &filename);
};
