#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <regex>
#include <ctime>
#include <sstream>
#include <cctype>
#include <windows.h>

void Utils::clearScreen() {
    system("cls");
}

void Utils::printHeader(const std::string& title) {
    printSeparator('=');
    std::cout << "║  " << title << std::endl;
    printSeparator('=');
}

void Utils::printSeparator(char ch) {
    for (int i = 0; i < 40; i++) {
        std::cout << ch;
    }
    std::cout << std::endl;
}

void Utils::printError(const std::string& message) {
    std::cout << "[ERROR] " << message << std::endl;
}

void Utils::printSuccess(const std::string& message) {
    std::cout << "[SUCCESS] " << message << std::endl;
}

void Utils::printInfo(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

void Utils::printWarning(const std::string& message) {
    std::cout << "[WARNING] " << message << std::endl;
}

void Utils::printResult(const std::string& label, const std::string& value) {
    std::cout << "  " << label << ": " << value << std::endl;
}

std::string Utils::getUserInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return trimString(input);
}

int Utils::getUserChoice(int min_choice, int max_choice) {
    int choice = -1;
    std::string input;
    
    while (choice < min_choice || choice > max_choice) {
        std::cout << "\nEnter your choice [" << min_choice << "-" << max_choice << "]: ";
        std::getline(std::cin, input);
        
        try {
            choice = std::stoi(input);
            if (choice < min_choice || choice > max_choice) {
                printError("Invalid choice! Please try again.");
                choice = -1;
            }
        } catch (...) {
            printError("Invalid input! Please enter a number.");
        }
    }
    
    return choice;
}

double Utils::stringToDouble(const std::string& str) {
    try {
        return std::stod(str);
    } catch (...) {
        return 0.0;
    }
}

int Utils::stringToInt(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (...) {
        return 0;
    }
}

bool Utils::isValidDomain(const std::string& domain) {
    if (domain.empty() || domain.length() > 255) return false;
    
    std::regex domainRegex(
        R"(^(?:[a-z0-9](?:[a-z0-9-]{0,61}[a-z0-9])?\.)*[a-z0-9](?:[a-z0-9-]{0,61}[a-z0-9])?$|^localhost$)",
        std::regex::icase
    );
    
    return std::regex_match(domain, domainRegex);
}

bool Utils::isValidIPv4(const std::string& ip) {
    std::regex ipv4Regex(
        R"(^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)"
    );
    
    return std::regex_match(ip, ipv4Regex);
}

bool Utils::isValidIP(const std::string& ip) {
    return isValidIPv4(ip);
}

std::string Utils::trimString(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> Utils::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trimString(token));
    }
    
    return tokens;
}

std::string Utils::toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string Utils::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void Utils::pauseConsole() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

bool Utils::isAdministrator() {
    BOOL is_admin = FALSE;
    HANDLE token = NULL;
    
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        TOKEN_ELEVATION elevation;
        DWORD size = sizeof(TOKEN_ELEVATION);
        
        if (GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size)) {
            is_admin = elevation.TokenIsElevated;
        }
        
        CloseHandle(token);
    }
    
    return is_admin == TRUE;
}

std::string Utils::getCurrentTime() {
    time_t now = time(0);
    char buf[80];
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return std::string(buf);
}

std::string Utils::getSystemInfo() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    
    std::stringstream ss;
    ss << "Processors: " << sysInfo.dwNumberOfProcessors;
    return ss.str();
}

std::string Utils::getComputerName() {
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    
    if (GetComputerNameA(computerName, &size)) {
        return std::string(computerName);
    }
    
    return "Unknown";
}

std::string Utils::getUserName() {
    char userName[UNLEN + 1];
    DWORD size = sizeof(userName);
    
    if (GetUserNameA(userName, &size)) {
        return std::string(userName);
    }
    
    return "Unknown";
}