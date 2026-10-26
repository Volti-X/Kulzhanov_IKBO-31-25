#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

vector<string> parseInput(const string& line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    const string vfsName = "my_vfs";
    string inputLine;

    while (true) {
        cout << vfsName << "> ";
        if (!getline(cin, inputLine)) break;

        vector<string> tokens = parseInput(inputLine);
        if (tokens.empty()) continue;

        const string& command = tokens[0];

        if (command == "exit") {
            break;
        }
        else if (command == "ls" || command == "cd") {
            cout << "command: " << command;
            if (tokens.size() > 1) {
                cout << " | arguments: ";
                for (size_t i = 1; i < tokens.size(); ++i) {
                    cout << tokens[i] << " ";
                }
            }
            else {
                cout << " | no arguments";
            }
            cout << "\n";
        }
        else {
            cout << "command not found: " << command << "\n";
        }
    }
    return 0;
}