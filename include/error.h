#pragma once
#include <vector>
#include <string>

inline std::vector<std::string> errors;

inline void throwError(std::string message){
    errors.push_back(message);
}