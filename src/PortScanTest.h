#pragma once

#include <string>
#include <vector>

struct PortInfo {
    int port;
    std::string service;
    bool isOpen;
    std::string status;
};

class PortScanTest {
public:
    static std::vector<PortInfo> scanPorts(const std::string& target, int startPort, int endPort);
    static void displayScanResults(const std::string& target, const std::vector<PortInfo>& ports);
    
private:
    static bool isPortOpen(const std::string& target, int port, int timeout = 1000);
    static std::string getServiceName(int port);
};