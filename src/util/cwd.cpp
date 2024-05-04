#include "cwd.h"

std::string get_cwd() { 
    char c_current_path[FILENAME_MAX];

    if (!GetCurrentDir(c_current_path, sizeof(c_current_path))) {
    }

    c_current_path[sizeof(c_current_path) - 1] = '\0';

    std::string s_current_path(c_current_path);

    return s_current_path;
}