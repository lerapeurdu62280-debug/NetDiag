#include "PortScanTest.h"
#include "Utils.h"
#include <iostream>
#include <winsock2.h>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")

std::vector<PortInfo> PortScanTest::scanPorts(const std::string& target, int startPort, int endPort) {
    std::vector<PortInfo> ports;
    
    for (int port = startPort; port <= endPort; port++) {
        if (isPortOpen(target, port)) {
            PortInfo pInfo;
            pInfo.port = port;
            pInfo.service = getServiceName(port);
            pInfo.isOpen = true;
            pInfo.status = "Open";
            ports.push_back(pInfo);
            
            std::cout << "Port " << port << " is OPEN\n";
        }
    }
    
    return ports;
}

bool PortScanTest::isPortOpen(const std::string& target, int port, int timeout) {
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        return false;
    }
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }
    
    // Set non-blocking mode
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
    
    struct hostent* phost = gethostbyname(target.c_str());
    if (phost == NULL) {
        closesocket(sock);
        WSACleanup();
        return false;
    }
    
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = *(u_long*)phost->h_addr_list[0];
    
    int result = connect(sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    
    bool isOpen = (result == 0);
    
    closesocket(sock);
    WSACleanup();
    
    return isOpen;
}

std::string PortScanTest::getServiceName(int port) {
    switch (port) {
        case 21: return "FTP";
        case 22: return "SSH";
        case 23: return "Telnet";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 80: return "HTTP";
        case 110: return "POP3";
        case 143: return "IMAP";
        case 443: return "HTTPS";
        case 3306: return "MySQL";
        case 3389: return "RDP";
        case 5432: return "PostgreSQL";
        case 8080: return "HTTP-Alt";
        default: return "Unknown";
    }
}

void PortScanTest::displayScanResults(const std::string& target, const std::vector<PortInfo>& ports) {
    Utils::printHeader("Port Scan Results");
    
    Utils::printResult("Target", target);
    std::cout << "\nOpen Ports:\n";
    
    if (ports.empty()) {
        Utils::printWarning("No open ports found!");
    } else {
        std::cout << std::left << std::setw(10) << "Port"
                  << std::setw(20) << "Service"
                  << std::setw(15) << "Status" << "\n";
        Utils::printSeparator('-');
        
        for (const auto& port : ports) {
            std::cout << std::left
                      << std::setw(10) << port.port
                      << std::setw(20) << port.service
                      << std::setw(15) << port.status << "\n";
        }
    }
    
    std::cout << "\n";
}