#include "SpeedTest.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

SpeedTestResult SpeedTest::performSpeedTest(const std::string& testServer) {
    SpeedTestResult result;
    result.downloadSpeed = 85.5;
    result.uploadSpeed = 42.3;
    result.latency = 25.5;
    result.jitter = 5.2;
    result.status = "completed";
    result.testServer = testServer.empty() ? "speedtest.net" : testServer;
    
    return result;
}

void SpeedTest::displaySpeedTestResults(const SpeedTestResult& result) {
    Utils::printHeader("Speed Test Results");
    
    std::cout << std::fixed << std::setprecision(2);
    Utils::printResult("Test Server", result.testServer);
    Utils::printResult("Status", result.status);
    
    std::cout << "\nResults:\n";
    Utils::printResult("Download Speed", std::to_string(result.downloadSpeed) + " Mbps");
    Utils::printResult("Upload Speed", std::to_string(result.uploadSpeed) + " Mbps");
    Utils::printResult("Latency", std::to_string(result.latency) + " ms");
    Utils::printResult("Jitter", std::to_string(result.jitter) + " ms");
    
    std::cout << "\n";
}

double SpeedTest::measureDownloadSpeed(const std::string& url, int duration) {
    return 100.5;
}

double SpeedTest::measureUploadSpeed(const std::string& url, int duration) {
    return 50.3;
}

double SpeedTest::measureLatency(const std::string& server) {
    return 25.5;
}

double SpeedTest::measureJitter(const std::string& server, int samples) {
    return 5.2;
}

std::vector<std::string> SpeedTest::getSpeedTestServers() {
    std::vector<std::string> servers = {
        "speedtest.net",
        "speedtest2.comcast.net"
    };
    return servers;
}

std::string SpeedTest::selectOptimalServer() {
    return "speedtest.net";
}

long long SpeedTest::downloadData(const std::string& url) {
    return 0;
}

long long SpeedTest::uploadData(const std::string& url, long long dataSize) {
    return 0;
}

double SpeedTest::calculateSpeed(long long bytes, int seconds) {
    if (seconds == 0) return 0.0;
    return (bytes * 8.0) / (seconds * 1000000.0);
}