#include "TracerouteTest.h"
#include "Utils.h"
#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "icmpapi.lib")

std::vector<TracerouteHop> TracerouteTest::performTraceroute(const std::string& target, int maxHops, int timeout) {
    std::vector<TracerouteHop> hops;
    
    IPAddr ipaddr = inet_addr(target.c_str());
    if (ipaddr == INADDR_NONE) {
        struct hostent* phe = gethostbyname(target.c_str());
        if (phe == NULL) {
            TracerouteHop hop;
            hop.hopNumber = 1;
            hop.status = "Failed to resolve hostname";
            hops.push_back(hop);
            return hops;
        }
        ipaddr = *(u_long*)phe->h_addr_list[0];
    }
    
    for (int ttl = 1; ttl <= maxHops; ttl++) {
        IP_OPTION_INFORMATION IpOptions;
        IpOptions.Ttl = ttl;
        IpOptions.Tos = 0;
        IpOptions.Flags = 0;
        IpOptions.OptionsSize = 0;
        IpOptions.OptionsData = NULL;
        
        DWORD ReplySize = sizeof(ICMP_ECHO_REPLY) + 8;
        void* ReplyBuffer = malloc(ReplySize);
        
        HANDLE hIcmpFile = IcmpSendEcho(NULL, ipaddr, NULL, 0, &IpOptions);
        if (hIcmpFile != NULL) {
            PICMP_ECHO_REPLY pEchoReply = (void*)ReplyBuffer;
            struct in_addr ReplyAddr;
            ReplyAddr.S_un.S_addr = pEchoReply->Address;
            
            TracerouteHop hop;
            hop.hopNumber = ttl;
            hop.ipAddress = inet_ntoa(ReplyAddr);
            hop.time1 = pEchoReply->Status;
            hop.status = "OK";
            
            hops.push_back(hop);
            
            IcmpCloseHandle(hIcmpFile);
            
            if (pEchoReply->Address == ipaddr) {
                break;
            }
        }
        
        free(ReplyBuffer);
    }
    
    return hops;
}

void TracerouteTest::displayTracerouteResults(const std::vector<TracerouteHop>& hops) {
    Utils::printHeader("Traceroute Results");
    
    std::cout << std::left << std::setw(5) << "Hop"
              << std::setw(20) << "IP Address"
              << std::setw(20) << "Time (ms)"
              << std::setw(15) << "Status" << "\n";
    
    Utils::printSeparator('-');
    
    for (const auto& hop : hops) {
        std::cout << std::left 
                  << std::setw(5) << hop.hopNumber
                  << std::setw(20) << hop.ipAddress
                  << std::setw(20) << hop.time1
                  << std::setw(15) << hop.status << "\n";
    }
    
    std::cout << "\n";
}