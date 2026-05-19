#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <chrono>

struct PacketInfo {
    std::chrono::system_clock::time_point timestamp;
    std::string sourceIP;
    std::string destIP;
    uint16_t sourcePort;
    uint16_t destPort;
    std::string protocol; // TCP, UDP, ICMP, etc.
    uint32_t length;
    std::string info;
};

class PacketCapture {
public:
    PacketCapture();
    ~PacketCapture();
    
    bool startCapture(const std::string& interfaceName = "", int packetCount = 100);
    bool stopCapture();
    std::vector<PacketInfo> getPackets() const;
    void clearPackets();
    bool isCapturing() const;
    int getPacketCount() const { return packets.size(); }
    
private:
    std::vector<PacketInfo> packets;
    bool capturing;
    int maxPackets;
    
    bool capturePackets();
    PacketInfo parsePacket(const uint8_t* data, uint32_t length);
};