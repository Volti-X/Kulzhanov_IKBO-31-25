#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

void processCmd(const vector<string>& tokens) {
    if (tokens.empty()) return;

    const string& command = tokens[0];

    if (command == "exit") {
        exit(0);
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


vector<string> parseInput(const string& line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    string vfsName = "my_vfs";
    string vfsPath = "";
    string scriptPath = "";

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--vfs" && i + 1 < argc) {
            vfsPath = argv[++i];
        }
        else if (arg == "--script" && i + 1 < argc) {
            scriptPath = argv[++i];
        }
    }

    cout << "=== Configuration ===" << "\n";
    cout << "VFS Path: " << (vfsPath.empty() ? "not set" : vfsPath) << "\n";
    cout << "Script Path: " << (scriptPath.empty() ? "not set" : scriptPath) << "\n";
    cout << "=====================" << "\n";

    if (!scriptPath.empty()) {
        ifstream scriptFile(scriptPath);
        if (!scriptFile.is_open()) {
            cerr << "Error: Cannot open script file: " << scriptPath << "\n";
            return 1;
        }

        string line;
        cout << "\n=== Executing script: " << scriptPath << " ===" << "\n";

        while (getline(scriptFile, line)) {
            if (line.empty() || line.find("//") == 0) continue;
            cout << vfsName << "> " << line << "\n";
            vector<string> tokens = parseInput(line);
            processCmd(tokens);
        }
        cout << "=== Script finished ===" << "\n";
    }
    else {
        string inputLine;
        while (true) {
            cout << vfsName << "> ";
            if (!getline(cin, inputLine)) break;

            vector<string> tokens = parseInput(inputLine);
            processCmd(tokens);
        }
    }
    return 0;
}