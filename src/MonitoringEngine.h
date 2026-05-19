#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <map>
#include <queue>
#include <chrono>

struct NetworkMetrics {
    std::chrono::system_clock::time_point timestamp;
    double bandwidth;
    double latency;
    int packetLoss;
    int activeConnections;
    double cpuUsage;
    double memoryUsage;
};

class MonitoringEngine {
public:
    MonitoringEngine();
    ~MonitoringEngine();
    
    void startMonitoring();
    void stopMonitoring();
    void setInterval(int milliseconds);
    std::vector<NetworkMetrics> getMetrics(int lastNSeconds = 60);
    NetworkMetrics getCurrentMetrics();
    void clearMetrics();
    bool isRunning() const { return running; }
    
private:
    void monitoringThread();
    NetworkMetrics captureMetrics();
    double calculateBandwidth();
    double calculateLatency();
    int calculatePacketLoss();
    int getActiveConnections();
    double getCPUUsage();
    double getMemoryUsage();
    
    std::atomic<bool> running;
    std::thread monitorThread;
    std::mutex metricsMutex;
    std::queue<NetworkMetrics> metricsHistory;
    int samplingInterval; // milliseconds
    int maxHistorySize;
};