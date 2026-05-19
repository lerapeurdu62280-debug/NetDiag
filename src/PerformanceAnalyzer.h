#pragma once

#include <string>
#include <vector>
#include <map>
#include "MonitoringEngine.h"

struct PerformanceReport {
    double avgBandwidth;
    double maxBandwidth;
    double minBandwidth;
    double avgLatency;
    double maxLatency;
    double minLatency;
    int avgPacketLoss;
    int maxPacketLoss;
    std::string overallStatus; // Good, Fair, Poor
    std::vector<std::string> recommendations;
};

class PerformanceAnalyzer {
public:
    static PerformanceReport analyzeMetrics(const std::vector<NetworkMetrics>& metrics);
    static std::string getHealthStatus(double latency, int packetLoss, double bandwidth);
    static std::vector<std::string> generateRecommendations(const PerformanceReport& report);
    static std::string exportReportText(const PerformanceReport& report);
    
private:
    static double calculateAverage(const std::vector<double>& values);
    static double calculateMax(const std::vector<double>& values);
    static double calculateMin(const std::vector<double>& values);
};