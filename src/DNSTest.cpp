#include "DNSTest.h"
#include "Utils.h"
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

std::vector<std::string> DNSTest::dnsLookupForward(const std::string& domain) {
    std::vector<std::string> ips;
    
    WSAData wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        return ips;
    }
    
    struct hostent* phost = gethostbyname(domain.c_str());
    if (phost == NULL) {
        WSACleanup();
        return ips;
    }
    
    for (int i = 0; phost->h_addr_list[i] != NULL; i++) {
        struct in_addr addr;
        addr.s_addr = *(u_long*)phost->h_addr_list[i];
        ips.push_back(std::string(inet_ntoa(addr)));
    }
    
    WSACleanup();
    return ips;
}

std::string DNSTest::dnsLookupReverse(const std::string& ip) {
    std::string domain = "";
    
    WSAData wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        return "Failed to initialize WSA";
    }
    
    unsigned long addr = inet_addr(ip.c_str());
    if (addr == INADDR_NONE) {
        WSACleanup();
        return "Invalid IP address";
    }
    
    struct hostent* phost = gethostbyaddr((char*)&addr, 4, AF_INET);
    if (phost == NULL) {
        WSACleanup();
        return "Could not resolve";
    }
    
    domain = std::string(phost->h_name);
    WSACleanup();
    return domain;
}

void DNSTest::displayDNSResults(const std::string& domain, const std::vector<std::string>& ips) {
    Utils::printHeader("DNS Forward Lookup Results");
    
    Utils::printResult("Domain", domain);
    std::cout << "\nResolved IP Addresses:\n";
    
    if (ips.empty()) {
        Utils::printWarning("No IP addresses found!");
    } else {
        for (size_t i = 0; i < ips.size(); i++) {
            std::cout << "  [" << (i + 1) << "] " << ips[i] << "\n";
        }
    }
    
    std::cout << "\n";
}

void DNSTest::displayReverseDNSResults(const std::string& ip, const std::string& domain) {
    Utils::printHeader("DNS Reverse Lookup Results");
    
    Utils::printResult("IP Address", ip);
    Utils::printResult("Domain", domain);
    std::cout << "\n";
}