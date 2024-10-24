//
// Created by Yucheng Soku on 2024/10/21.
//

#include <string>
#include <iostream>
#include "Config.h"
#include "Utils/PyTrigger.h"

bool pyTrigger(const char* scriptPath, const char* envName) {

    std::string script_path = PYTHON_SCRIPT_DIR + std::string(scriptPath);
    std::string command = "conda run -n " + std::string(envName) + " python " + script_path;

    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "\n>>> Executing Python Script < " << scriptPath << " >  Successfully\n" << std::endl;
    } else {
        std::cerr << "\n>>> Failing to Execute Python Script\n" << std::endl;
    }

    return result != 0;
}