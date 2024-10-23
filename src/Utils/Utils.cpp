//
// Created by Yucheng Soku on 2024/10/22.
//

#include "Utils/Utils.h"

void printProgress(float progress) {
    int barWidth = 50;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) {
            std::cout << "#";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "]" << int(progress * 100.0) << "%\r";
    std::cout.flush();
}