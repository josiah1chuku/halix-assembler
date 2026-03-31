// =============================================================================
// utils.cpp — Utility function implementations
// =============================================================================

#include "utils.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

using std::string;
using std::vector;

// ---------------------------------------------------------------------------
string stripComment(const string& raw) {
    size_t pos = raw.find('#');
    string s   = (pos != string::npos) ? raw.substr(0, pos) : raw;
    while (!s.empty() && isspace(static_cast<unsigned char>(s.back())))
        s.pop_back();
    return s;
}

// ---------------------------------------------------------------------------
vector<string> tokenize(const string& line) {
    vector<string> tokens;
    std::istringstream ss(line);
    string tok;
    while (ss >> tok) tokens.push_back(tok);
    return tokens;
}

// ---------------------------------------------------------------------------
bool isValidImmediate(const string& s) {
    if (s.size() < 2 || s[0] != '=') return false;
    string rest  = s.substr(1);
    size_t start = (rest[0] == '-') ? 1 : 0;
    if (start >= rest.size()) return false;   // "=-" with no digits
    for (size_t i = start; i < rest.size(); i++)
        if (!isdigit(static_cast<unsigned char>(rest[i]))) return false;
    return true;
}

// ---------------------------------------------------------------------------
bool isValidIdentifier(const string& s) {
    if (s.empty()) return false;
    if (!isalpha(static_cast<unsigned char>(s[0])) && s[0] != '_') return false;
    for (char c : s)
        if (!isalnum(static_cast<unsigned char>(c)) && c != '_') return false;
    return true;
}

// ---------------------------------------------------------------------------
string getBaseName(const string& path) {
    size_t slash = path.find_last_of("/\\");
    string name  = (slash == string::npos) ? path : path.substr(slash + 1);
    size_t dot   = name.rfind('.');
    return (dot == string::npos) ? name : name.substr(0, dot);
}

// ---------------------------------------------------------------------------
bool readSourceFile(const string& file, vector<string>& lines) {
    std::ifstream in(file);
    if (!in.is_open()) return false;
    string raw;
    while (std::getline(in, raw)) lines.push_back(raw);
    return true;
}

// ---------------------------------------------------------------------------
bool sourceContainsDirective(const vector<string>& lines, const string& directive) {
    for (const string& raw : lines) {
        auto toks = tokenize(stripComment(raw));
        if (!toks.empty() && toks[0] == directive) return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
bool parseInt(const string& s, int& out) {
    try   { out = std::stoi(s); return true; }
    catch (...) { return false; }
}
