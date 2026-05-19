#include "PingTest.h"
#include "Utils.h"
#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "icmpapi.lib")

PingStatistics PingTest::performPing(const std::string& target, int count, int timeout) {
    PingStatistics stats;
    stats.target = target;
    stats.packetsSent = count;
    stats.packetsReceived = 0;
    stats.minTime = 999999;
    stats.maxTime = 0;
    stats.avgTime = 0;
    
    // Convert target to IP
    IPAddr ipaddr = inet_addr(target.c_str());
    if (ipaddr == INADDR_NONE) {
        // Try to resolve hostname
        struct hostent* phe = gethostbyname(target.c_str());
        if (phe == NULL) {
            stats.status = "Failed to resolve hostname";
            return stats;
        }
        ipaddr = *(u_long*)phe->h_addr_list[0];
    }
    
    // Create ICMP handle
    HANDLE hIcmpFile = IcmpSendEcho(NULL, ipaddr, NULL, 0, NULL);
    if (hIcmpFile == NULL) {
        stats.status = "Unable to open handle";
        return stats;
    }
    
    // Send pings
    double totalTime = 0;
    for (int i = 0; i < count; i++) {
        DWORD ReplySize = sizeof(ICMP_ECHO_REPLY) + 8;
        void* ReplyBuffer = malloc(ReplySize);
        
        if (ReplyBuffer == NULL) {
            stats.status = "Memory allocation failed";
            IcmpCloseHandle(hIcmpFile);
            return stats;
        }
        
        DWORD dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, NULL, 0, (PIP_OPTION_INFORMATION)NULL);
        
        if (dwRetVal != 0) {
            PICMP_ECHO_REPLY pEchoReply = (void*)ReplyBuffer;
            struct in_addr ReplyAddr;
            ReplyAddr.S_un.S_addr = pEchoReply->Address;
            
            stats.packetsReceived++;
            double time = (double)pEchoReply->Status;
            
            if (time < stats.minTime) stats.minTime = time;
            if (time > stats.maxTime) stats.maxTime = time;
            totalTime += time;
            
            std::cout << "Reply from " << target << ": bytes=32 time=" << (long)time << "ms TTL=64\n";
        } else {
            std::cout << "Request timed out\n";
        }
        
        free(ReplyBuffer);
    }
    
    IcmpCloseHandle(hIcmpFile);
    
    if (stats.packetsReceived > 0) {
        stats.avgTime = totalTime / stats.packetsReceived;
        stats.packetLoss = (int)calculatePacketLoss(stats.packetsSent, stats.packetsReceived);
        stats.status = "Success";
    } else {
        stats.packetLoss = 100;
        stats.status = "All packets lost";
    }
    
    return stats;
}

void PingTest::displayPingResults(const PingStatistics& stats) {
    Utils::printHeader("Ping Statistics");
    
    std::cout << std::fixed << std::setprecision(2);
    Utils::printResult("Target", stats.target);
    Utils::printResult("Packets Sent", std::to_string(stats.packetsSent));
    Utils::printResult("Packets Received", std::to_string(stats.packetsReceived));
    Utils::printResult("Packet Loss", std::to_string(stats.packetLoss) + "%");
    
    if (stats.packetsReceived > 0) {
        Utils::printResult("Min Time", std::to_string(stats.minTime) + " ms");
        Utils::printResult("Max Time", std::to_string(stats.maxTime) + " ms");
        Utils::printResult("Avg Time", std::to_string(stats.avgTime) + " ms");
    }
    
    Utils::printResult("Status", stats.status);
    std::cout << "\n";
}

double PingTest::calculatePacketLoss(int sent, int received) {
    if (sent == 0) return 0.0;
    return ((double)(sent - received) / sent) * 100.0;
}