#include "MonitoringEngine.h"
#include <iostream>
#include <windows.h>
#include <iphlpapi.h>
#include <psapi.h>
#include <thread>
#include <chrono>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "psapi.lib")

MonitoringEngine::MonitoringEngine() 
    : running(false), samplingInterval(1000), maxHistorySize(3600) {}

MonitoringEngine::~MonitoringEngine() {
    stopMonitoring();
}

void MonitoringEngine::startMonitoring() {
    if (running) return;
    running = true;
    monitorThread = std::thread(&MonitoringEngine::monitoringThread, this);
}

void MonitoringEngine::stopMonitoring() {
    running = false;
    if (monitorThread.joinable()) {
        monitorThread.join();
    }
}

void MonitoringEngine::setInterval(int milliseconds) {
    samplingInterval = milliseconds;
}

void MonitoringEngine::monitoringThread() {
    while (running) {
        NetworkMetrics metrics = captureMetrics();
        
        {
            std::lock_guard<std::mutex> lock(metricsMutex);
            metricsHistory.push(metrics);
            
            if (metricsHistory.size() > maxHistorySize) {
                metricsHistory.pop();
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(samplingInterval));
    }
}

NetworkMetrics MonitoringEngine::captureMetrics() {
    NetworkMetrics metrics;
    metrics.timestamp = std::chrono::system_clock::now();
    metrics.bandwidth = calculateBandwidth();
    metrics.latency = calculateLatency();
    metrics.packetLoss = calculatePacketLoss();
    metrics.activeConnections = getActiveConnections();
    metrics.cpuUsage = getCPUUsage();
    metrics.memoryUsage = getMemoryUsage();
    
    return metrics;
}

std::vector<NetworkMetrics> MonitoringEngine::getMetrics(int lastNSeconds) {
    std::vector<NetworkMetrics> result;
    std::lock_guard<std::mutex> lock(metricsMutex);
    
    auto now = std::chrono::system_clock::now();
    std::queue<NetworkMetrics> temp = metricsHistory;
    
    while (!temp.empty()) {
        const auto& metric = temp.front();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - metric.timestamp).count();
        
        if (elapsed <= lastNSeconds) {
            result.push_back(metric);
        }
        
        temp.pop();
    }
    
    return result;
}

NetworkMetrics MonitoringEngine::getCurrentMetrics() {
    std::lock_guard<std::mutex> lock(metricsMutex);
    
    if (metricsHistory.empty()) {
        return NetworkMetrics();
    }
    
    // Create a temporary queue to access the last element
    std::queue<NetworkMetrics> temp = metricsHistory;
    NetworkMetrics last = temp.front();
    
    while (!temp.empty()) {
        last = temp.front();
        temp.pop();
    }
    
    return last;
}

void MonitoringEngine::clearMetrics() {
    std::lock_guard<std::mutex> lock(metricsMutex);
    while (!metricsHistory.empty()) {
        metricsHistory.pop();
    }
}

double MonitoringEngine::calculateBandwidth() {
    // Simplified bandwidth calculation
    // In production, use GetIpStatistics
    return 45.5; // Mbps placeholder
}

double MonitoringEngine::calculateLatency() {
    // Placeholder: ~20-30ms typical latency
    return 25.3;
}

int MonitoringEngine::calculatePacketLoss() {
    // Placeholder: ~0-2% typical loss
    return 0;
}

int MonitoringEngine::getActiveConnections() {
    // Using netstat data
    return 42; // Placeholder
}

double MonitoringEngine::getCPUUsage() {
    HANDLE hProcess = GetCurrentProcess();
    FILETIME ftCreation, ftExit, ftKernel, ftUser;
    
    if (GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
        ULARGE_INTEGER uKernel, uUser;
        uKernel.LowPart = ftKernel.dwLowDateTime;
        uKernel.HighPart = ftKernel.dwHighDateTime;
        uUser.LowPart = ftUser.dwLowDateTime;
        uUser.HighPart = ftUser.dwHighDateTime;
        
        return (double)(uKernel.QuadPart + uUser.QuadPart) / 10000000.0;
    }
    
    return 0.0;
}

double MonitoringEngine::getMemoryUsage() {
    HANDLE hProcess = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        return (double)pmc.WorkingSetSize / (1024 * 1024); // Convert to MB
    }
    
    return 0.0;
}
