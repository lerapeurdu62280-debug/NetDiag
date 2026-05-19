#include "PerformanceAnalyzer.h"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>

PerformanceReport PerformanceAnalyzer::analyzeMetrics(const std::vector<NetworkMetrics>& metrics) {
    PerformanceReport report;
    
    if (metrics.empty()) {
        report.avgBandwidth = 0;
        report.avgLatency = 0;
        report.avgPacketLoss = 0;
        report.overallStatus = "No data";
        return report;
    }
    
    std::vector<double> bandwidths, latencies;
    std::vector<int> packetLosses;
    
    for (const auto& metric : metrics) {
        bandwidths.push_back(metric.bandwidth);
        latencies.push_back(metric.latency);
        packetLosses.push_back(metric.packetLoss);
    }
    
    report.avgBandwidth = calculateAverage(bandwidths);
    report.maxBandwidth = calculateMax(bandwidths);
    report.minBandwidth = calculateMin(bandwidths);
    
    report.avgLatency = calculateAverage(latencies);
    report.maxLatency = calculateMax(latencies);
    report.minLatency = calculateMin(latencies);
    
    report.avgPacketLoss = (int)std::accumulate(packetLosses.begin(), packetLosses.end(), 0) / packetLosses.size();
    report.maxPacketLoss = *std::max_element(packetLosses.begin(), packetLosses.end());
    
    report.overallStatus = getHealthStatus(report.avgLatency, report.avgPacketLoss, report.avgBandwidth);
    report.recommendations = generateRecommendations(report);
    
    return report;
}

std::string PerformanceAnalyzer::getHealthStatus(double latency, int packetLoss, double bandwidth) {
    if (latency > 100 || packetLoss > 5 || bandwidth < 10) {
        return "POOR";
    } else if (latency > 50 || packetLoss > 2 || bandwidth < 30) {
        return "FAIR";
    } else {
        return "GOOD";
    }
}

std::vector<std::string> PerformanceAnalyzer::generateRecommendations(const PerformanceReport& report) {
    std::vector<std::string> recommendations;
    
    if (report.avgLatency > 100) {
        recommendations.push_back("High latency detected. Consider checking your network connection.");
    }
    
    if (report.avgPacketLoss > 2) {
        recommendations.push_back("Significant packet loss detected. Check for network interference or faulty cables.");
    }
    
    if (report.avgBandwidth < 10) {
        recommendations.push_back("Low bandwidth. Consider contacting your ISP or checking background applications.");
    }
    
    if (report.overallStatus == "GOOD") {
        recommendations.push_back("Network performance is optimal. No issues detected.");
    }
    
    return recommendations;
}

std::string PerformanceAnalyzer::exportReportText(const PerformanceReport& report) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "=== NETWORK PERFORMANCE REPORT ===\n\n";
    ss << "Overall Status: " << report.overallStatus << "\n\n";
    ss << "Bandwidth Analysis:\n";
    ss << "  Average: " << report.avgBandwidth << " Mbps\n";
    ss << "  Maximum: " << report.maxBandwidth << " Mbps\n";
    ss << "  Minimum: " << report.minBandwidth << " Mbps\n\n";
    ss << "Latency Analysis:\n";
    ss << "  Average: " << report.avgLatency << " ms\n";
    ss << "  Maximum: " << report.maxLatency << " ms\n";
    ss << "  Minimum: " << report.minLatency << " ms\n\n";
    ss << "Packet Loss:\n";
    ss << "  Average: " << report.avgPacketLoss << "%\n";
    ss << "  Maximum: " << report.maxPacketLoss << "%\n\n";
    ss << "Recommendations:\n";
    for (const auto& rec : report.recommendations) {
        ss << "  - " << rec << "\n";
    }
    
    return ss.str();
}

double PerformanceAnalyzer::calculateAverage(const std::vector<double>& values) {
    if (values.empty()) return 0;
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double PerformanceAnalyzer::calculateMax(const std::vector<double>& values) {
    if (values.empty()) return 0;
    return *std::max_element(values.begin(), values.end());
}

double PerformanceAnalyzer::calculateMin(const std::vector<double>& values) {
    if (values.empty()) return 0;
    return *std::min_element(values.begin(), values.end());
}
