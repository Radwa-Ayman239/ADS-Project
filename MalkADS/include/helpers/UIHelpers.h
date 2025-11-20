//
// Created by hamdy on 11/20/2025.
//

#ifndef MALKADS_UIHELPERS_H
#define MALKADS_UIHELPERS_H

#include <iostream>
#include <string>
using namespace std;

// Simple ANSI color codes for nicer CLI (works in most terminals)
static const string COLOR_RESET = "\033[0m";
static const string COLOR_TITLE = "\033[1;36m"; // bright cyan
static const string COLOR_MENU = "\033[1;33m"; // bright yellow
static const string COLOR_PROMPT = "\033[1;32m"; // bright green
static const string COLOR_ERROR = "\033[1;31m"; // bright red
static const string COLOR_DIM = "\033[2m"; // dim gray

static constexpr int UI_WIDTH = 78; // line width for headers/boxes

static void printLine(const char ch = '=') {
    cout << string(UI_WIDTH, ch) << "\n";
}

static void printCentered(const string &text, const string &color = COLOR_TITLE) {
    int pad = (UI_WIDTH - static_cast<int>(text.size())) / 2;
    if (pad < 0) pad = 0;
    cout << color << string(pad, ' ') << text << COLOR_RESET << "\n";
}

static void clearScreen() {
    // Basic ANSI clear; safe even if terminal doesn't fully support it
    cout << "\033[2J\033[H";
}

// UIHelpers.h

static void printSectionHeader(const string &title) {
    clearScreen();
    printLine();
    printCentered(title);
    printLine();
}

static void printHint(const string &text) {
    cout << COLOR_DIM << text << COLOR_RESET << "\n";
}

static void printError(const string &text) {
    cout << "\n" << COLOR_ERROR << text << COLOR_RESET << "\n";
}

static void printSuccess(const string &text) {
    cout << "\n" << COLOR_PROMPT << text << COLOR_RESET << "\n";
}


#endif //MALKADS_UIHELPERS_H
