// This file is for rough work/ideas
#include <conio.h>

#include <iostream>
#include <string>
using namespace std;

void func1(void) { cout << "I am func1" << endl; }

void func2(void) { cout << "I am func2" << endl; }

void suggest(std::string str) {};
std::string get_input(void) {
    std::cout << "[I]: ";
    std::string input_str = "";

    for (int i = 0; i != 13; i = _getch()) {
        // The ascii codes for printable/normal chars lie between 32 and 126,
        // 32 represents space and 126 represents a tilde ~
        // if the getch() function returns an int out of this range then we
        // know that a special key was pressed and we may need to getch() one
        // more time to know which special key was pressed because special keys
        // often send signal which are 2 character width long.
        if (i > 32 && i < 126) {
            input_str.push_back((char)i);
            std::cout << (char)i;
        } else {
            // This whole block is to handle special keys
            if (i == 8) {
                if (!input_str.empty()) {
                    std::cout << "\b \b";
                    input_str.pop_back();
                }

            } else if (i == 9) {
                suggest(input_str);
            } else {
                char _ = _getch();
            }
        }
    }
    return input_str;
}

// Problem: To provide suggestions as the user is typing input and
//          auto-complete based on suggestions, this can be done by pressing
//          tab key and switching through the suggestions
// Solution:
//          A wrapper around the input function?

int main(void) {
    std::string istr = get_input();
    std::cout << "\ninput was: " << istr << "\n";
}