#pragma once

#include <string>
#include <vector>
#include "PingTest.h"
#include "MonitoringEngine.h"

struct ExportData {
    std::string testName;
    std::string timestamp;
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
};

class DataExporter {
public:
    // CSV Export
    static bool exportToCsv(const std::string& filename, const PingStatistics& stats);
    static bool exportMonitoringToCsv(const std::string& filename, const std::vector<NetworkMetrics>& metrics);
    
    // JSON Export
    static bool exportToJson(const std::string& filename, const PingStatistics& stats);
    static bool exportMonitoringToJson(const std::string& filename, const std::vector<NetworkMetrics>& metrics);
    
    // Generic export
    static bool exportGeneric(const std::string& filename, const ExportData& data, const std::string& format);
    
private:
    static std::string sanitizeString(const std::string& str);
    static std::string generateJsonHeader();
    static std::string generateJsonFooter();
};