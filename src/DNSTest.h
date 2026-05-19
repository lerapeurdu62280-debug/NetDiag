#pragma once

#include <string>
#include <vector>

class DNSTest {
public:
    static std::vector<std::string> dnsLookupForward(const std::string& domain);
    static std::string dnsLookupReverse(const std::string& ip);
    static void displayDNSResults(const std::string& domain, const std::vector<std::string>& ips);
    static void displayReverseDNSResults(const std::string& ip, const std::string& domain);
};