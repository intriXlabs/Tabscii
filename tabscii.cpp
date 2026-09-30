#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>

class tabscii {
private:
    struct rgbColor {
        int red;
        int green;
        int blue;
    };

private:
    int originRow = 0;
    int originCol = 0;

    int tabWidth = 10; // default width of the tab
    int tabHeight = 10; // default height of the tab

    rgbColor textColor = {255, 255, 255}; // default white text

    rgbColor tabFgColor = {0, 0, 0};      // default black foreground
    rgbColor tabBgColor = {255, 255, 255}; // default white background

    int terminalRows;
    int terminalCols;

public:    
    enum boxType {
        SIMPLE_BOX,
        EDGE_BOUNDARY_BOX,
        EDGE_SIDED_BOUNDARY_BOX
    };
    boxType currentBoxType = SIMPLE_BOX;

private:
    std::string helperPosition(int row, int col){return "\033[" + std::to_string(row) + ";" + std::to_string(col) + "H";}
    std::string helperFgColor(int red, int green, int blue){return "\033[38;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";}
    std::string helperBgColor(int red, int green, int blue){return "\033[48;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";}

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

        //adding color to the box
        std::cout << helperFgColor(fgRed, fgGreen, fgBlue);
        std::cout << helperBgColor(bgRed, bgGreen, bgBlue);
        
        // main loop to draw the box
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

        //adding color to the box
        std::cout << helperFgColor(fgRed, fgGreen, fgBlue);
        std::cout << helperBgColor(bgRed, bgGreen, bgBlue);
        
        // main loop to draw the box
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
        
        //adding color to the box
        std::cout << helperFgColor(fgRed, fgGreen, fgBlue);
        std::cout << helperBgColor(bgRed, bgGreen, bgBlue);
        
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

private:
    size_t visibleTextWidth(const std::string& value) const {
        size_t width = 0;
        bool inEscape = false;

        for (char ch : value) {
            unsigned char c = static_cast<unsigned char>(ch);
            if (inEscape) {
                if (c >= 0x40 && c <= 0x7E) {
                    inEscape = false;
                }
                continue;
            }

            if (c == 0x1B) {
                inEscape = true;
                continue;
            }

            ++width;
        }

        return width;
    }

    std::vector<std::string> wrapTextToRectangle(const std::string& input, int maxWidth) const {
        std::vector<std::string> wrappedLines;
        if (maxWidth <= 0) {
            return wrappedLines;
        }

        std::string currentLine;
        std::string currentWord;

        auto pushCurrentLine = [&]() {
            if (!currentLine.empty()) {
                wrappedLines.push_back(currentLine);
                currentLine.clear();
            }
        };

        auto flushWord = [&]() {
            if (currentWord.empty()) {
                return;
            }

            if ((int)visibleTextWidth(currentWord) <= maxWidth) {
                if (currentLine.empty()) {
                    currentLine = currentWord;
                } else if ((int)(visibleTextWidth(currentLine) + 1 + visibleTextWidth(currentWord)) <= maxWidth) {
                    currentLine += " " + currentWord;
                } else {
                    pushCurrentLine();
                    currentLine = currentWord;
                }
            } else {
                size_t split = 0;
                while (split < currentWord.size()) {
                    size_t chunkLen = 0;
                    while (split + chunkLen < currentWord.size()) {
                        std::string candidate = currentWord.substr(split, chunkLen + 1);
                        if ((int)visibleTextWidth(candidate) > maxWidth) {
                            break;
                        }
                        ++chunkLen;
                    }

                    if (chunkLen == 0) {
                        chunkLen = 1;
                    }

                    if (!currentLine.empty()) {
                        pushCurrentLine();
                    }
                    wrappedLines.push_back(currentWord.substr(split, chunkLen));
                    split += chunkLen;
                }
            }

            currentWord.clear();
        };

        for (char ch : input) {
            if (ch == '\n' || ch == '\r') {
                flushWord();
                pushCurrentLine();
                continue;
            }

            if (std::isspace(static_cast<unsigned char>(ch))) {
                flushWord();
                continue;
            }

            currentWord += ch;
        }

        flushWord();
        pushCurrentLine();

        return wrappedLines;
    }

    int makeText(int row, int col, int width, int height, std::string text, std::string fgHex, std::string bgHex, std::string specialDecorations = "") {
        int fgRed, fgGreen, fgBlue;
        int bgRed, bgGreen, bgBlue;

        convertHexToRGB(fgHex, fgRed, fgGreen, fgBlue);
        convertHexToRGB(bgHex, bgRed, bgGreen, bgBlue);

        if (width <= 0 || height <= 0) {
            return 0;
        }

        std::vector<std::string> wrappedLines = wrapTextToRectangle(text, width);
        int linesToPrint = std::min(height, (int)wrappedLines.size());

        for (int i = 0; i < linesToPrint; i++) {
            std::cout << helperPosition(row + i, col);
            std::cout << helperFgColor(fgRed, fgGreen, fgBlue);
            std::cout << helperBgColor(bgRed, bgGreen, bgBlue);
            std::cout << specialDecorations;
            std::cout << wrappedLines[i];
            int padding = width - static_cast<int>(visibleTextWidth(wrappedLines[i]));
            for (int j = 0; j < padding; j++) {
                std::cout << ' ';
            }
            std::cout << "\033[0m";
        }

        return 0;
    }

private:
    int refreshTerminalSize() {
        // Get terminal size using ioctl
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
            perror("ioctl");
            return -1;
        }
        terminalRows = w.ws_row;
        terminalCols = w.ws_col;
        return 0;
    }

public:
    tabscii() {
        refreshTerminalSize();
        // tab height and width is by default because - there are going to be two modes:: autoTab() and manualTab()
        // and auto tab going to do the job of calculating the height and width of the tab based on the terminal size and text title and discription text etc.
    }

    int setBoxType(boxType type) {
        currentBoxType = type;
        return 0;
    }

    int generateAutoTab(int row, int col, std::string title, std::string description, std::string fgHex, std::string bgHex, std::string titleDecorations = "\033[4m", std::string descriptionDecorations = "") {
        if (refreshTerminalSize() == -1) {
            return -1; // Error refreshing terminal size
        }
        if(row < 0 || col < 0 || row >= terminalRows || col >= terminalCols) {
            std::cerr << "Error: Row and column must be within the terminal size." << std::endl;
            return -1;
        }
        
        int titleLength = title.length();
        int descriptionLength = description.length();

        // adding colors to elements of the tab, so that we can use them later to print the tab with colors.
        int fgRed, fgGreen, fgBlue;
        int bgRed, bgGreen, bgBlue;
        convertHexToRGB(fgHex, fgRed, fgGreen, fgBlue);
        convertHexToRGB(bgHex, bgRed, bgGreen, bgBlue);
        tabFgColor = {fgRed, fgGreen, fgBlue};
        tabBgColor = {bgRed, bgGreen, bgBlue};
        textColor = {fgRed, fgGreen, fgBlue};


        // max width of tab is considered by simple formula, discription can be max upto four times of the title length. it's auto tab generation so yeah.. btw i adding a variable so you can change it. 
        int maxWidthHelperVar = 4; // you can change this variable to any number you want, it will change the max length of the tab.
        int maxWidth = maxWidthHelperVar * titleLength; // max length of the tab is twice the title length + a variable which you can change to any number you want.

        // calculate the width of the tab based on the title and description length
        tabWidth = maxWidth + 4; // 4 is for padding and borders
        
        // tab height is getting calculated based on the title and description length, and also the max width of the tab.
        tabHeight = descriptionLength / maxWidth + 4; // 4 is for padding and borders, and also the max width of the tab is considered to be twice the title length.



        switch(currentBoxType) {
            case SIMPLE_BOX:
                makeSimpleBox(row, col, tabWidth, tabHeight, fgHex, bgHex);
                break;
            case EDGE_BOUNDARY_BOX:
                makeEdgeBoundaryBox(row, col, tabWidth, tabHeight, fgHex, bgHex);
                break;
            case EDGE_SIDED_BOUNDARY_BOX:
                makeEdgeSidedBoundaryBox(row, col, tabWidth, tabHeight, fgHex, bgHex, "left-right");
                break;
        }

        // calculating the position to place the title to fit it in center of the tab
        int titleRow = row + 1; // 1 row below the top border of the tab
        int titleCol = col + (tabWidth - titleLength) / 2; // center the title in the tab

        // placing the title in the tab
        makeText(titleRow, titleCol, titleLength, 1, title, fgHex, bgHex, titleDecorations); // \033[4m is for underlined text

        // plkacing the description in the tab, starting from the row below the title
        int descriptionRow = titleRow + 2; // 2 rows below the title
        int descriptionCol = col + 2; // 2 columns right to the left border of the tab

        // placing the description in the tab
        makeText(descriptionRow, descriptionCol, maxWidth, tabHeight - 4, description, fgHex, bgHex, descriptionDecorations); // \033[4m is for underlined text

        std::cout << std::flush; // flush the output to the terminal
        return 0;
    }

};

int main() {
    tabscii tab;
    tab.generateAutoTab(5, 10, "My Tab", "This is a description of the tab. and this is a very long description that will be used to test the auto tab generation.", 
        "#00a8a2", "#ffffff");
    tab.setBoxType(tabscii::EDGE_BOUNDARY_BOX);
    tab.generateAutoTab(5, 50, "Another Tab", "This is another description of the tab. and this is a very long description that will be used to test the auto tab generation.",
         "#ff8585", "#000000");
    tab.setBoxType(tabscii::EDGE_SIDED_BOUNDARY_BOX);
    tab.generateAutoTab(5, 110, "Third Tab", "This is the third description of the tab. and this is a very long description that will be used to test the auto tab generation.",
         "#004c00", "#ffffb9");  
    std::cout << "\n\n\n\n" << std::endl; // Move to the next line after drawing the tab
    

    return 0;
}