#pragma once

#include <string>
#include <vector>

struct NetworkAdapter {
    std::string name;
    std::string description;
    std::string ipAddress;
    std::string subnetMask;
    std::string gateway;
    std::string dnsServers;
    std::string macAddress;
    bool isActive;
};

class NetworkInfo {
public:
    static void displayNetworkInfo();
    static std::vector<NetworkAdapter> getNetworkAdapters();
    static std::string getPublicIP();
    static std::string getDNSServers();
    static std::string getGateway();
    static bool isInternetConnected();
};