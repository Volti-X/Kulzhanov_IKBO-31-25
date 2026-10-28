#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include "json.hpp"

using namespace std;

struct VfsNode {
    string name;
    bool isDir = true;
    string data = "";
    vector<VfsNode> children;
};

VfsNode vfsRoot;
VfsNode currentDir;
string vfsName = "my_vfs";
bool isVfsLoaded = false;

VfsNode parseNode(const string& name, const nlohmann::json& j) {
    VfsNode node;
    node.name = name;

    if (j.contains("type") && j["type"] == "file") {
        node.isDir = false;
        if (j.contains("data"))
            node.data = j["data"];
    }
    else {
        if (j.contains("children")) {
            for (auto it = j["children"].begin(); it != j["children"].end(); ++it) {
                node.children.push_back(parseNode(it.key(), it.value()));
            }
        }
    }
    return node;
}

bool loadVfs(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error: Cannot open VFS file: " << path << "\n";
        return false;
    }
    try {
        nlohmann::json j;
        file >> j;
        if (j.contains("name"))
            vfsName = j["name"];

        if (j.contains("root")) {
            vfsRoot = parseNode("root", j["root"]);
            currentDir = vfsRoot;
        }
        for (size_t i = 0; i < vfsRoot.children.size(); ++i) {
            if (vfsRoot.children[i].name == "motd" && !vfsRoot.children[i].isDir) {
                cout << "\n[MOTD]:\n" << vfsRoot.children[i].data << "\n\n";
                break;
            }
        }
        isVfsLoaded = true;
        return true;
    }
    catch (const exception& e) {
        cerr << "Error parsing JSON: " << e.what() << "\n";
        return false;
    }
}

void processCmd(const vector<string>& tokens) {
    if (tokens.empty()) return;

    const string& command = tokens[0];

    if (command == "exit") {
        exit(0);
    }
    else if (command == "ls") {
        if (!isVfsLoaded) {
            cout << "Error: No VFS loaded\n";
            return;
        }
        for (size_t i = 0; i < currentDir.children.size(); ++i) {
            cout << currentDir.children[i].name;
            if (currentDir.children[i].isDir) {
                cout << "/";
            }
            cout << "\n";
        }
    }
    else if (command == "cd") {
        if (tokens.size() < 2) {
            cout << "cd: missing argument\n";
            return;
        }
        string target = tokens[1];

        if (target == "/" || target == "\\") {
            currentDir = vfsRoot;
            return;
        }

        bool found = false;
        for (size_t i = 0; i < currentDir.children.size(); ++i) {
            if (currentDir.children[i].name == target && currentDir.children[i].isDir) {
                currentDir = currentDir.children[i];
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "cd: no such directory: " << target << "\n";
        }
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

    if (!vfsPath.empty()) {
        if (!loadVfs(vfsPath)) {
            return 1;
        }
    }
    else {
        cout << "Warning: No VFS loaded. Commands will be limited.\n";
    }

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