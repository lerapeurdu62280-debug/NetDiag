#pragma once

#include <string>
#include <vector>

struct SpeedTestResult {
    double downloadSpeed;
    double uploadSpeed;
    double latency;
    double jitter;
    std::string status;
    std::string testServer;
};

class SpeedTest {
public:
    static SpeedTestResult performSpeedTest(const std::string& testServer = "");
    static void displaySpeedTestResults(const SpeedTestResult& result);
    
private:
    static double measureDownloadSpeed(const std::string& url, int duration);
    static double measureUploadSpeed(const std::string& url, int duration);
    static double measureLatency(const std::string& server);
    static double measureJitter(const std::string& server, int samples);
    static std::vector<std::string> getSpeedTestServers();
    static std::string selectOptimalServer();
    static long long downloadData(const std::string& url);
    static long long uploadData(const std::string& url, long long dataSize);
    static double calculateSpeed(long long bytes, int seconds);
};