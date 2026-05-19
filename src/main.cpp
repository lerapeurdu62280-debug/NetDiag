#include <iostream>
#include <string>
#include "Utils.h"
#include "NetworkInfo.h"
#include "PingTest.h"
#include "DNSTest.h"
#include "TracerouteTest.h"
#include "PortScanTest.h"
#include "SpeedTest.h"

void displayMainMenu() {
    Utils::clearScreen();
    Utils::printHeader("NetDiag - Network Diagnostic Tool");
    
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║           MAIN MENU                    ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    std::cout << "1. Network Information\n";
    std::cout << "2. Ping Test\n";
    std::cout << "3. DNS Lookup\n";
    std::cout << "4. Traceroute\n";
    std::cout << "5. Port Scan\n";
    std::cout << "6. Speed Test\n";
    std::cout << "7. System Information\n";
    std::cout << "0. Exit\n";
}

void handleNetworkInfo() {
    Utils::clearScreen();
    NetworkInfo::displayNetworkInfo();
    Utils::pauseConsole();
}

void handlePingTest() {
    Utils::clearScreen();
    Utils::printHeader("Ping Test");
    
    std::string target = Utils::getUserInput("Enter target (domain or IP): ");
    if (!Utils::isValidDomain(target) && !Utils::isValidIP(target)) {
        Utils::printError("Invalid target format!");
        Utils::pauseConsole();
        return;
    }
    
    std::cout << "\nPinging " << target << "...\n\n";
    PingStatistics stats = PingTest::performPing(target, 4, 4000);
    PingTest::displayPingResults(stats);
    
    Utils::pauseConsole();
}

void handleDNSTest() {
    Utils::clearScreen();
    Utils::printHeader("DNS Lookup");
    
    std::cout << "\n1. Forward Lookup (Domain -> IP)\n";
    std::cout << "2. Reverse Lookup (IP -> Domain)\n";
    int choice = Utils::getUserChoice(1, 2);
    std::cout << "\n";
    
    if (choice == 1) {
        std::string domain = Utils::getUserInput("Enter domain name: ");
        if (!Utils::isValidDomain(domain)) {
            Utils::printError("Invalid domain format!");
            Utils::pauseConsole();
            return;
        }
        
        std::cout << "\nLooking up " << domain << "...\n";
        std::vector<std::string> ips = DNSTest::dnsLookupForward(domain);
        DNSTest::displayDNSResults(domain, ips);
    }
    else if (choice == 2) {
        std::string ip = Utils::getUserInput("Enter IP address: ");
        if (!Utils::isValidIP(ip)) {
            Utils::printError("Invalid IP format!");
            Utils::pauseConsole();
            return;
        }
        
        std::cout << "\nResolving " << ip << "...\n";
        std::string domain = DNSTest::dnsLookupReverse(ip);
        DNSTest::displayReverseDNSResults(ip, domain);
    }
    
    Utils::pauseConsole();
}

void handleTraceroute() {
    Utils::clearScreen();
    Utils::printHeader("Traceroute");
    
    std::string target = Utils::getUserInput("Enter target (domain or IP): ");
    if (!Utils::isValidDomain(target) && !Utils::isValidIP(target)) {
        Utils::printError("Invalid target format!");
        Utils::pauseConsole();
        return;
    }
    
    std::cout << "\nTracing route to " << target << "...\n\n";
    std::vector<TracerouteHop> hops = TracerouteTest::performTraceroute(target, 30, 5000);
    TracerouteTest::displayTracerouteResults(hops);
    
    Utils::pauseConsole();
}

void handlePortScan() {
    Utils::clearScreen();
    Utils::printHeader("Port Scan");
    
    if (!Utils::isAdministrator()) {
        Utils::printWarning("Port scanning works best with Administrator privileges!");
    }
    
    std::string target = Utils::getUserInput("Enter target (domain or IP): ");
    if (!Utils::isValidDomain(target) && !Utils::isValidIP(target)) {
        Utils::printError("Invalid target format!");
        Utils::pauseConsole();
        return;
    }
    
    int startPort = Utils::stringToInt(Utils::getUserInput("Enter starting port (default 1): "));
    if (startPort <= 0) startPort = 1;
    
    int endPort = Utils::stringToInt(Utils::getUserInput("Enter ending port (default 1000): "));
    if (endPort <= 0) endPort = 1000;
    if (endPort < startPort) std::swap(startPort, endPort);
    
    std::cout << "\nScanning ports " << startPort << " to " << endPort << "...\n\n";
    std::vector<PortInfo> openPorts = PortScanTest::scanPorts(target, startPort, endPort);
    PortScanTest::displayScanResults(target, openPorts);
    
    Utils::pauseConsole();
}

void handleSpeedTest() {
    Utils::clearScreen();
    Utils::printHeader("Speed Test");
    
    std::cout << "Starting speed test...\n\n";
    SpeedTestResult result = SpeedTest::performSpeedTest();
    SpeedTest::displaySpeedTestResults(result);
    
    Utils::pauseConsole();
}

void handleSystemInfo() {
    Utils::clearScreen();
    Utils::printHeader("System Information");
    
    Utils::printResult("Current Time", Utils::getCurrentTime());
    Utils::printResult("Administrator", Utils::isAdministrator() ? "Yes" : "No");
    Utils::printResult("System", Utils::getSystemInfo());
    Utils::printResult("Computer Name", Utils::getComputerName());
    Utils::printResult("User Name", Utils::getUserName());
    
    std::cout << "\n";
    Utils::pauseConsole();
}

int main() {
    int choice = -1;
    
    while (true) {
        displayMainMenu();
        choice = Utils::getUserChoice(0, 7);
        
        switch (choice) {
            case 1:
                handleNetworkInfo();
                break;
            case 2:
                handlePingTest();
                break;
            case 3:
                handleDNSTest();
                break;
            case 4:
                handleTraceroute();
                break;
            case 5:
                handlePortScan();
                break;
            case 6:
                handleSpeedTest();
                break;
            case 7:
                handleSystemInfo();
                break;
            case 0:
                Utils::clearScreen();
                std::cout << "Thank you for using NetDiag!\n";
                std::cout << "Developed by S.O.S INFO LUDO\n";
                return 0;
            default:
                Utils::printError("Invalid choice!");
                Utils::pauseConsole();
        }
    }
    
    return 0;
}