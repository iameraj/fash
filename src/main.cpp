#include <conio.h>
#include <windows.h>

#include <iostream>
#include <map>
#include <unordered_map>

#include "fash_impl_win.cpp"

// [TODO]
void run_command(const char& ch, fash obj) { return; }

int main(void) {
    // [TODO] homeDir is meant to be the starting point for the shell, the
    // getenv("USERPROFILE") function may not provive a proper path across
    // multiple version of windows or if the user has disrupted the env
    // variables
    // std::string* homeDir = (std::string*)getenv("USERPROFILE");
    // if (homeDir == nullptr) {
    //     std::cout << "Error retrieving home directory." << std::endl;
    //     return 1;
    // }
    std::string homeDir = getenv("USERPROFILE");

    // intializing fash to a safe dummy directory [for now]
    fash session(homeDir + "\\fash");

    std::cout << "Welcome to Super Shell" << std::endl;
    std::cout << "press x to exit" << std::endl;

    // main loop
    for (char ch = '~'; ch != 'X'; ch = _getch()) {
        ch = toupper(ch);

        if (ch == 'L') {
            session.list_dir();
        } else if (ch == 'C') {
            session.clear_scr();
        } else if (ch == 'M') {
            session.make_dir();
        } else if (ch == 'R') {
            session.remove_dir();
        } else if (ch == 'F') {
            session.change_dir();
        } else if (ch == 'Y') {
            session.copy_item();
        }
    }
    // do {
    //     char ch = _getch();
    //     if (ch == 'L') {
    //     }
    // } while (ch != 'X');

    _putch('\n');  // Line feed
}