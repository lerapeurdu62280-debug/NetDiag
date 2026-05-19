#pragma once

#include <string>
#include <vector>

struct PingStatistics {
    std::string target;
    int packetsSent;
    int packetsReceived;
    int packetLoss;
    double minTime;
    double maxTime;
    double avgTime;
    std::string status;
};

class PingTest {
public:
    static PingStatistics performPing(const std::string& target, int count = 4, int timeout = 4000);
    static void displayPingResults(const PingStatistics& stats);
    
private:
    static double calculatePacketLoss(int sent, int received);
};