#pragma once

#include <string>
#include <vector>

struct TracerouteHop {
    int hopNumber;
    std::string ipAddress;
    std::string hostName;
    double time1;
    double time2;
    double time3;
    std::string status;
};

class TracerouteTest {
public:
    static std::vector<TracerouteHop> performTraceroute(const std::string& target, int maxHops = 30, int timeout = 5000);
    static void displayTracerouteResults(const std::vector<TracerouteHop>& hops);
};