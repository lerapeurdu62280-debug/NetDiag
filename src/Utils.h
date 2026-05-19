#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <windows.h>

class Utils {
public:
    // Console functions
    static void clearScreen();
    static void printHeader(const std::string& title);
    static void printSeparator(char ch = '=');
    static void printError(const std::string& message);
    static void printSuccess(const std::string& message);
    static void printInfo(const std::string& message);
    static void printWarning(const std::string& message);
    static void printResult(const std::string& label, const std::string& value);
    
    // Input functions
    static std::string getUserInput(const std::string& prompt);
    static int getUserChoice(int min_choice, int max_choice);
    static double stringToDouble(const std::string& str);
    static int stringToInt(const std::string& str);
    
    // Validation functions
    static bool isValidDomain(const std::string& domain);
    static bool isValidIP(const std::string& ip);
    static bool isValidIPv4(const std::string& ip);
    
    // String functions
    static std::string trimString(const std::string& str);
    static std::vector<std::string> splitString(const std::string& str, char delimiter);
    static std::string toUpperCase(const std::string& str);
    static std::string toLowerCase(const std::string& str);
    
    // System functions
    static void pauseConsole();
    static bool isAdministrator();
    static std::string getCurrentTime();
    static std::string getSystemInfo();
    static std::string getComputerName();
    static std::string getUserName();
};