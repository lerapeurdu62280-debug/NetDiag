#include "DataExporter.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

bool DataExporter::exportToCsv(const std::string& filename, const PingStatistics& stats) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    // Write headers
    file << "Target,Packets Sent,Packets Received,Packet Loss (%),Min Time (ms),Max Time (ms),Avg Time (ms),Status\n";
    
    // Write data
    file << sanitizeString(stats.target) << ","
         << stats.packetsSent << ","
         << stats.packetsReceived << ","
         << stats.packetLoss << ","
         << std::fixed << std::setprecision(2) << stats.minTime << ","
         << stats.maxTime << ","
         << stats.avgTime << ","
         << sanitizeString(stats.status) << "\n";
    
    file.close();
    return true;
}

bool DataExporter::exportMonitoringToCsv(const std::string& filename, const std::vector<NetworkMetrics>& metrics) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    // Write headers
    file << "Timestamp,Bandwidth (Mbps),Latency (ms),Packet Loss (%),Active Connections,CPU Usage (%),Memory Usage (MB)\n";
    
    // Write data
    for (const auto& metric : metrics) {
        auto time = std::chrono::system_clock::to_time_t(metric.timestamp);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
        
        file << timeStr << ","
             << std::fixed << std::setprecision(2) << metric.bandwidth << ","
             << metric.latency << ","
             << metric.packetLoss << ","
             << metric.activeConnections << ","
             << metric.cpuUsage << ","
             << metric.memoryUsage << "\n";
    }
    
    file.close();
    return true;
}

bool DataExporter::exportToJson(const std::string& filename, const PingStatistics& stats) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    file << generateJsonHeader();
    file << "  \"testType\": \"ping\",\n";
    file << "  \"data\": {\n";
    file << "    \"target\": \"" << sanitizeString(stats.target) << "\",\n";
    file << "    \"packetsSent\": " << stats.packetsSent << ",\n";
    file << "    \"packetsReceived\": " << stats.packetsReceived << ",\n";
    file << "    \"packetLoss\": " << stats.packetLoss << ",\n";
    file << "    \"minTime\": " << std::fixed << std::setprecision(2) << stats.minTime << ",\n";
    file << "    \"maxTime\": " << stats.maxTime << ",\n";
    file << "    \"avgTime\": " << stats.avgTime << ",\n";
    file << "    \"status\": \"" << sanitizeString(stats.status) << "\"\n";
    file << "  }\n";
    file << generateJsonFooter();
    
    file.close();
    return true;
}

bool DataExporter::exportMonitoringToJson(const std::string& filename, const std::vector<NetworkMetrics>& metrics) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    file << generateJsonHeader();
    file << "  \"testType\": \"monitoring\",\n";
    file << "  \"dataPoints\": " << metrics.size() << ",\n";
    file << "  \"data\": [\n";
    
    for (size_t i = 0; i < metrics.size(); i++) {
        const auto& metric = metrics[i];
        auto time = std::chrono::system_clock::to_time_t(metric.timestamp);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
        
        file << "    {\n";
        file << "      \"timestamp\": \"" << timeStr << "\",\n";
        file << "      \"bandwidth\": " << std::fixed << std::setprecision(2) << metric.bandwidth << ",\n";
        file << "      \"latency\": " << metric.latency << ",\n";
        file << "      \"packetLoss\": " << metric.packetLoss << ",\n";
        file << "      \"activeConnections\": " << metric.activeConnections << ",\n";
        file << "      \"cpuUsage\": " << metric.cpuUsage << ",\n";
        file << "      \"memoryUsage\": " << metric.memoryUsage << "\n";
        file << "    }";
        
        if (i < metrics.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "  ]\n";
    file << generateJsonFooter();
    
    file.close();
    return true;
}

bool DataExporter::exportGeneric(const std::string& filename, const ExportData& data, const std::string& format) {
    if (format == "csv") {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        
        // Write headers
        for (size_t i = 0; i < data.headers.size(); i++) {
            file << data.headers[i];
            if (i < data.headers.size() - 1) file << ",";
        }
        file << "\n";
        
        // Write rows
        for (const auto& row : data.rows) {
            for (size_t i = 0; i < row.size(); i++) {
                file << sanitizeString(row[i]);
                if (i < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        
        file.close();
        return true;
    }
    
    return false;
}

std::string DataExporter::sanitizeString(const std::string& str) {
    std::string result = str;
    if (result.find(',') != std::string::npos || result.find('"') != std::string::npos) {
        result = "\"" + result + "\"";
    }
    return result;
}

std::string DataExporter::generateJsonHeader() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    
    std::string header = "{\n  \"exportTime\": \"";
    header += timeStr;
    header += "\",\n";
    return header;
}

std::string DataExporter::generateJsonFooter() {
    return "}\n";
}
