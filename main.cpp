#include <iostream> 
#include <string>
#include <vector>

std::string helperPosition(int row, int col){
    return "\033[" + std::to_string(row) + ";" + std::to_string(col) + "H";
}

std::string helperFgColor(int red, int green, int blue){
    return "\033[38;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";
}

std::string helperBgColor(int red, int green, int blue){
    return "\033[48;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";
}

void convertHexToRGB(const std::string& hex, int& red, int& green, int& blue) {
    if (hex.length() != 7 || hex[0] != '#') {
        throw std::invalid_argument("Invalid hex color format. Use #RRGGBB.");
    }
    red = std::stoi(hex.substr(1, 2), nullptr, 16);
    green = std::stoi(hex.substr(3, 2), nullptr, 16);
    blue = std::stoi(hex.substr(5, 2), nullptr, 16);
}

int makeSimpleBox(int row, int col, int width, int height, std::string fgHex, std::string bgHex) {
    int fgRed, fgGreen, fgBlue;
    int bgRed, bgGreen, bgBlue;

    convertHexToRGB(fgHex, fgRed, fgGreen, fgBlue);
    convertHexToRGB(bgHex, bgRed, bgGreen, bgBlue);

    // string to hold the box
    std::string boxLeft = "▌";
    std::string boxRight = "▐";
    std::string boxUpper = "▀";
    std::string boxLower = "▄";
    std::string boxCorners[4] = {"▛", "▜", "▙", "▟"};
    
    // helper strings
    std::string fgColor = helperFgColor(fgRed, fgGreen, fgBlue);
    std::string bgColor = helperBgColor(bgRed, bgGreen, bgBlue);

    //adding color to the box
    std::cout << fgColor;
    std::cout << bgColor;
    
    for(int i = 0; i < height; i++){
        std::cout << helperPosition(row + i, col);
        if(i == 0){
            std::cout << boxCorners[0];
            for(int j = 0; j < width - 2; j++){
                std::cout << boxUpper;
            }
            std::cout << boxCorners[1];
        } else if(i == height - 1){
            std::cout << boxCorners[2];
            for(int j = 0; j < width - 2; j++){
                std::cout << boxLower;
            }
            std::cout << boxCorners[3];
        } else {
            std::cout << boxLeft;
            for(int j = 0; j < width - 2; j++){
                std::cout << " ";
            }
            std::cout << boxRight;
        }
    }

    // reset color
    std::cout << "\033[0m";

    return 0;

}

int makeEdgeBoundaryBox(int row, int col, int width, int height, std::string fgHex, std::string bgHex) {
    int fgRed, fgGreen, fgBlue;
    int bgRed, bgGreen, bgBlue;

    convertHexToRGB(fgHex, fgRed, fgGreen, fgBlue);
    convertHexToRGB(bgHex, bgRed, bgGreen, bgBlue);

    // string to hold the box
    std::string boxLeft = "▏";
    std::string boxRight = "▕";
    std::string boxupper = "▔";
    std::string boxLower = "▁";
    std::string boxCorners[4] = {"▛", "▜", "▙", "▟"};
    
    // helper strings
    std::string fgColor = helperFgColor(fgRed, fgGreen, fgBlue);
    std::string bgColor = helperBgColor(bgRed, bgGreen, bgBlue);

    //adding color to the box
    std::cout << fgColor;
    std::cout << bgColor;
    
    for(int i = 0; i < height; i++){
        std::cout << helperPosition(row + i, col);
        if(i == 0){
            std::cout << boxCorners[0];
            for(int j = 0; j < width - 2; j++){
                std::cout << boxupper;
            }
            std::cout << boxCorners[1];
        } else if(i == height - 1){
            std::cout << boxCorners[2];
            for(int j = 0; j < width - 2; j++){
                std::cout << boxLower;
            }
            std::cout << boxCorners[3];
        } else {
            std::cout << boxLeft;
            for(int j = 0; j < width - 2; j++){
                std::cout << " ";
            }
            std::cout << boxRight;
        }
    }

    // reset color
    std::cout << "\033[0m";

    return 0;

}

int makeEdgeSidedBoundaryBox(int row, int col, int width, int height, std::string fgHex, std::string bgHex, std::string boxMode = "left-right") {
    int fgRed, fgGreen, fgBlue;
    int bgRed, bgGreen, bgBlue;

    convertHexToRGB(fgHex, fgRed, fgGreen, fgBlue);
    convertHexToRGB(bgHex, bgRed, bgGreen, bgBlue);

    // string to hold the box
    std::string boxLeft = "▏";
    std::string boxRight = "▕";
    std::string boxupper = "▔";
    std::string boxLower = "▁";
    
    // helper strings
    std::string fgColor = helperFgColor(fgRed, fgGreen, fgBlue);
    std::string bgColor = helperBgColor(bgRed, bgGreen, bgBlue);

    //adding color to the box
    std::cout << fgColor;
    std::cout << bgColor;
    
    // covering full left and right sides of the box while algo leave spaces in middle
    if(boxMode == "left-right"){
        for(int i = 0; i < height; i++){
            std::cout << helperPosition(row + i, col);
            std::cout << boxLeft;
            for(int j = 0; j < width - 2; j++){
                std::cout << " ";
            }
            std::cout << boxRight;
        }
    } else if(boxMode == "top-bottom"){
        for(int i = 0; i < height; i++){
            std::cout << helperPosition(row + i, col);
            if(i == 0){
                for(int j = 0; j < width; j++){
                    std::cout << boxupper;
                }
            } else if(i == height - 1){
                for(int j = 0; j < width; j++){
                    std::cout << boxLower;
                }
            } else {
                for(int j = 0; j < width; j++){
                    std::cout << " ";
                }
            }
        }
    }

    // reset color
    std::cout << "\033[0m";

    return 0;

}


int main() {
    int row = 5;
    int col = 10;
    int width = 20;
    int height = 10;
    std::string fgHex = "#ffffff";
    std::string bgHex = "#000000";
    makeSimpleBox(row, col, width, height, fgHex, bgHex);

    row = 5;
    col = 40;
    width = 20;
    height = 10;
    fgHex = "#ffffff";
    bgHex = "#000000";

    makeEdgeBoundaryBox(row , col, width, height, fgHex, bgHex);

    row = 5;
    col = 70;
    width = 20;
    height = 10;
    fgHex = "#ffffff";
    bgHex = "#000000";

    makeEdgeSidedBoundaryBox(row , col, width, height, fgHex, bgHex, "left-right");

    row = 5;
    col = 100;
    width = 20;
    height = 10;
    fgHex = "#ffffff";
    bgHex = "#000000";
    
    makeEdgeSidedBoundaryBox(row , col, width, height, fgHex, bgHex, "top-bottom");

    row = 5;
    col = 130;
    width = 20;
    height = 10;
    fgHex = "#ffffff";
    bgHex = "#000000";

    return 0;
}