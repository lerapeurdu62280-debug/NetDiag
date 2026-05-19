#include "NetworkInfo.h"
#include "Utils.h"
#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

void NetworkInfo::displayNetworkInfo() {
    Utils::printHeader("Network Information");
    
    std::cout << "\n[Network Adapters]\n";
    auto adapters = getNetworkAdapters();
    
    if (adapters.empty()) {
        Utils::printWarning("No network adapters found!");
        return;
    }
    
    for (size_t i = 0; i < adapters.size(); i++) {
        std::cout << "\nAdapter " << (i + 1) << ":\n";
        Utils::printResult("Name", adapters[i].name);
        Utils::printResult("Description", adapters[i].description);
        Utils::printResult("IP Address", adapters[i].ipAddress);
        Utils::printResult("Subnet Mask", adapters[i].subnetMask);
        Utils::printResult("Gateway", adapters[i].gateway);
        Utils::printResult("DNS Servers", adapters[i].dnsServers);
        Utils::printResult("MAC Address", adapters[i].macAddress);
        Utils::printResult("Status", adapters[i].isActive ? "Active" : "Inactive");
    }
    
    std::cout << "\n[System DNS]\n";
    Utils::printResult("DNS Servers", getDNSServers());
    
    std::cout << "\n[Internet Connectivity]\n";
    bool connected = isInternetConnected();
    Utils::printResult("Internet Connected", connected ? "Yes" : "No");
}

std::vector<NetworkAdapter> NetworkInfo::getNetworkAdapters() {
    std::vector<NetworkAdapter> adapters;
    
    PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
    }
    
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR) {
        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
        
        while (pAdapter) {
            NetworkAdapter adapter;
            adapter.name = std::string(pAdapter->AdapterName);
            adapter.description = std::string(pAdapter->Description);
            adapter.ipAddress = std::string(pAdapter->IpAddressList.IpAddress.String);
            adapter.subnetMask = std::string(pAdapter->IpAddressList.IpMask.String);
            adapter.gateway = std::string(pAdapter->GatewayList.IpAddress.String);
            adapter.macAddress = "";
            adapter.isActive = (pAdapter->Type != IF_TYPE_PPP);
            
            for (UINT i = 0; i < pAdapter->AddressLength; i++) {
                char hex[3];
                sprintf_s(hex, "%02X", (int)pAdapter->Address[i]);
                adapter.macAddress += hex;
                if (i < pAdapter->AddressLength - 1) adapter.macAddress += "-";
            }
            
            adapters.push_back(adapter);
            pAdapter = pAdapter->Next;
        }
    }
    
    if (pAdapterInfo) {
        free(pAdapterInfo);
    }
    
    return adapters;
}

std::string NetworkInfo::getPublicIP() {
    return "N/A (requires internet call)";
}

std::string NetworkInfo::getDNSServers() {
    FIXED_INFO* pFixedInfo = (FIXED_INFO*)malloc(sizeof(FIXED_INFO));
    ULONG ulOutBufLen = sizeof(FIXED_INFO);
    
    if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pFixedInfo);
        pFixedInfo = (FIXED_INFO*)malloc(ulOutBufLen);
    }
    
    std::string dnsServers = "";
    
    if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) {
        PIP_ADDR_STRING pIPAddr = &pFixedInfo->DnsServerList;
        
        while (pIPAddr) {
            dnsServers += pIPAddr->IpAddress.String;
            if (pIPAddr->Next) dnsServers += ", ";
            pIPAddr = pIPAddr->Next;
        }
    }
    
    if (pFixedInfo) {
        free(pFixedInfo);
    }
    
    return dnsServers.empty() ? "Not configured" : dnsServers;
}

std::string NetworkInfo::getGateway() {
    PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
    }
    
    std::string gateway = "";
    
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR) {
        gateway = pAdapterInfo->GatewayList.IpAddress.String;
    }
    
    if (pAdapterInfo) {
        free(pAdapterInfo);
    }
    
    return gateway.empty() ? "Not found" : gateway;
}

bool NetworkInfo::isInternetConnected() {
    DWORD flags;
    return InternetGetConnectedState(&flags, 0) == TRUE;
}