#pragma once
// =============================================================================
// utils.h — Utility function declarations
// Pure helpers used by multiple passes; no assembler state required.
// =============================================================================

#include <string>
#include <vector>

// Strip everything from '#' to end-of-line; trim trailing whitespace.
std::string stripComment(const std::string& raw);

// Split a line into whitespace-delimited tokens.
std::vector<std::string> tokenize(const std::string& line);

// Return true if s matches  =<integer>  (optional leading '-', digits only).
// Rejects "=-" (no digits after minus).
bool isValidImmediate(const std::string& s);

// Return true if s is a valid C-style identifier:
// starts with letter or '_', followed by alnum or '_'.
bool isValidIdentifier(const std::string& s);

// Strip directory path and file extension, returning the base name.
// e.g. "tests/p0/tc_p0_01.hal"  →  "tc_p0_01"
std::string getBaseName(const std::string& path);

// Read every line of a text file into lines[].
// Returns false if the file cannot be opened.
bool readSourceFile(const std::string& file, std::vector<std::string>& lines);

// Scan already-loaded lines for a top-level directive token.
// Avoids re-opening the source file during pass0.
bool sourceContainsDirective(const std::vector<std::string>& lines,
                             const std::string& directive);

// Safe stoi wrapper — returns false and leaves out unchanged on any failure.
bool parseInt(const std::string& s, int& out);
