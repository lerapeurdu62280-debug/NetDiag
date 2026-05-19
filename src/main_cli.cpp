#include <iostream>
#include <string>
#include "Utils.h"
#include "NetworkInfo.h"
#include "PingTest.h"
#include "DNSTest.h"
#include "TracerouteTest.h"
#include "PortScanTest.h"
#include "SpeedTest.h"
#include "MonitoringEngine.h"
#include "DataExporter.h"
#include "PacketCapture.h"
#include "PerformanceAnalyzer.h"
#include <fstream>

MonitoringEngine* globalMonitor = nullptr;
PacketCapture* globalPacketCapture = nullptr;

void displayMainMenu() {
    Utils::clearScreen();
    Utils::printHeader("NetDiag PRO - Advanced Network Diagnostic Tool");
    
    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║           MAIN MENU - CLI MODE                 ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n\n";
    
    std::cout << "CORE TESTS:\n";
    std::cout << "1. Network Information\n";
    std::cout << "2. Ping Test\n";
    std::cout << "3. DNS Lookup\n";
    std::cout << "4. Traceroute\n";
    std::cout << "5. Port Scan\n";
    std::cout << "6. Speed Test\n\n";
    
    std::cout << "ADVANCED FEATURES:\n";
    std::cout << "7. Real-Time Monitoring\n";
    std::cout << "8. Packet Capture\n";
    std::cout << "9. Performance Analysis\n";
    std::cout << "10. Export Results\n\n";
    
    std::cout << "SYSTEM:\n";
    std::cout << "11. System Information\n";
    std::cout << "0. Exit\n";
}

void handleMonitoring() {
    Utils::clearScreen();
    Utils::printHeader("Real-Time Network Monitoring");
    
    int duration = Utils::stringToInt(Utils::getUserInput("Enter monitoring duration (seconds, default 30): "));
    if (duration <= 0) duration = 30;
    
    globalMonitor = new MonitoringEngine();
    globalMonitor->setInterval(1000); // 1 second interval
    globalMonitor->startMonitoring();
    
    std::cout << "\nMonitoring for " << duration << " seconds...\n\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << "Timestamp            | Bandwidth | Latency | Packet Loss | Connections | CPU  | Memory\n";
    std::cout << std::string(80, '=') << "\n";
    
    for (int i = 0; i < duration; i++) {
        NetworkMetrics metrics = globalMonitor->getCurrentMetrics();
        
        auto timePoint = std::chrono::system_clock::to_time_t(metrics.timestamp);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&timePoint));
        
        std::printf("%-20s | %9.2f | %7.2f | %11d | %12d | %5.2f | %6.2f MB\n",
            timeStr, metrics.bandwidth, metrics.latency, metrics.packetLoss,
            metrics.activeConnections, metrics.cpuUsage, metrics.memoryUsage);
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    globalMonitor->stopMonitoring();
    std::vector<NetworkMetrics> metrics = globalMonitor->getMetrics(duration);
    
    // Export option
    std::string exportChoice = Utils::getUserInput("\nExport monitoring data? (csv/json/none): ");
    if (exportChoice == "csv") {
        DataExporter::exportMonitoringToCsv("monitoring_data.csv", metrics);
        Utils::printSuccess("Data exported to monitoring_data.csv");
    } else if (exportChoice == "json") {
        DataExporter::exportMonitoringToJson("monitoring_data.json", metrics);
        Utils::printSuccess("Data exported to monitoring_data.json");
    }
    
    delete globalMonitor;
    Utils::pauseConsole();
}

void handlePacketCapture() {
    Utils::clearScreen();
    Utils::printHeader("Packet Capture");
    
    int packetCount = Utils::stringToInt(Utils::getUserInput("Number of packets to capture (default 100): "));
    if (packetCount <= 0) packetCount = 100;
    
    globalPacketCapture = new PacketCapture();
    globalPacketCapture->startCapture("", packetCount);
    
    std::cout << "\nCapturing " << packetCount << " packets...\n\n";
    std::cout << std::string(100, '=') << "\n";
    std::cout << "Timestamp            | Source IP        | Dest IP          | Src Port | Dst Port | Protocol | Length\n";
    std::cout << std::string(100, '=') << "\n";
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    globalPacketCapture->stopCapture();
    
    auto packets = globalPacketCapture->getPackets();
    for (const auto& packet : packets) {
        auto timePoint = std::chrono::system_clock::to_time_t(packet.timestamp);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&timePoint));
        
        std::printf("%-20s | %-16s | %-16s | %8d | %8d | %-8s | %6d\n",
            timeStr, packet.sourceIP.c_str(), packet.destIP.c_str(),
            packet.sourcePort, packet.destPort, packet.protocol.c_str(), packet.length);
    }
    
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << "Total packets captured: " << packets.size() << "\n";
    
    delete globalPacketCapture;
    Utils::pauseConsole();
}

void handlePerformanceAnalysis() {
    Utils::clearScreen();
    Utils::printHeader("Performance Analysis");
    
    if (!globalMonitor) {
        Utils::printWarning("No monitoring data available. Please run monitoring first.");
        Utils::pauseConsole();
        return;
    }
    
    std::vector<NetworkMetrics> metrics = globalMonitor->getMetrics(300);
    PerformanceReport report = PerformanceAnalyzer::analyzeMetrics(metrics);
    
    std::string reportText = PerformanceAnalyzer::exportReportText(report);
    std::cout << "\n" << reportText << "\n";
    
    std::string exportChoice = Utils::getUserInput("Export report? (txt/none): ");
    if (exportChoice == "txt") {
        std::ofstream file("performance_report.txt");
        file << reportText;
        file.close();
        Utils::printSuccess("Report exported to performance_report.txt");
    }
    
    Utils::pauseConsole();
}

void handleExportResults() {
    Utils::clearScreen();
    Utils::printHeader("Export Results");
    
    std::cout << "\n1. Export Ping Test\n";
    std::cout << "2. Export Monitoring Data\n";
    std::cout << "3. Performance Report\n";
    
    int choice = Utils::getUserChoice(1, 3);
    
    // Placeholder - implement based on actual test data
    
    Utils::pauseConsole();
}

int main() {
    int choice = -1;
    globalMonitor = nullptr;
    globalPacketCapture = nullptr;
    
    while (true) {
        displayMainMenu();
        choice = Utils::getUserChoice(0, 11);
        
        switch (choice) {
            case 1:
                handleNetworkInfo();
                break;
            case 2:
                handlePingTest();
                break;
            case 3:
                handleDNSTest();
                break;
            case 4:
                handleTraceroute();
                break;
            case 5:
                handlePortScan();
                break;
            case 6:
                handleSpeedTest();
                break;
            case 7:
                handleMonitoring();
                break;
            case 8:
                handlePacketCapture();
                break;
            case 9:
                handlePerformanceAnalysis();
                break;
            case 10:
                handleExportResults();
                break;
            case 11:
                handleSystemInfo();
                break;
            case 0:
                Utils::clearScreen();
                std::cout << "Thank you for using NetDiag PRO!\n";
                std::cout << "Developed by S.O.S INFO LUDO\n";
                return 0;
            default:
                Utils::printError("Invalid choice!");
                Utils::pauseConsole();
        }
    }
    
    return 0;
}
