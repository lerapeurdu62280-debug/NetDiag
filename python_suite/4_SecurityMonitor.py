#!/usr/bin/env python3
"""
SecurityMonitor v2.0 - Network Security & Threat Detection
Features: Port Scanning, Vulnerability Detection, Intrusion Detection, Threat Intelligence
Python 3.8+ | PyInstaller compatible
"""

import json
import socket
import threading
from datetime import datetime
from collections import defaultdict, deque
from typing import Dict, List, Tuple
import time

class SecurityMonitor:
    """Network security and threat detection"""
    
    def __init__(self):
        self.open_ports = []
        self.vulnerabilities = deque(maxlen=100)
        self.intrusions = deque(maxlen=100)
        self.threat_level = 'LOW'  # LOW, MEDIUM, HIGH, CRITICAL
        self.common_ports = {
            21: 'FTP',
            22: 'SSH',
            23: 'Telnet',
            25: 'SMTP',
            53: 'DNS',
            80: 'HTTP',
            110: 'POP3',
            143: 'IMAP',
            443: 'HTTPS',
            445: 'SMB',
            3306: 'MySQL',
            3389: 'RDP',
            5432: 'PostgreSQL',
            8080: 'HTTP-Alt',
            8443: 'HTTPS-Alt'
        }
        self.vulnerable_services = {
            'Telnet': 'CRITICAL',
            'FTP': 'HIGH',
            'HTTP': 'MEDIUM',
            'MySQL': 'MEDIUM',
            'SMB': 'HIGH'
        }
    
    def scan_port(self, host: str, port: int, timeout: int = 2) -> bool:
        """Scan single port"""
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(timeout)
            result = sock.connect_ex((host, port))
            sock.close()
            return result == 0
        except Exception as e:
            return False
    
    def scan_ports(self, host: str, start_port: int = 1, end_port: int = 1024, 
                   threads: int = 10) -> List[int]:
        """Scan port range with threading"""
        print(f"[INFO] Scanning {host}:{start_port}-{end_port} with {threads} threads...")
        
        self.open_ports = []
        lock = threading.Lock()
        
        def worker(port_range):
            for port in port_range:
                if self.scan_port(host, port):
                    with lock:
                        self.open_ports.append(port)
                    service = self.common_ports.get(port, 'Unknown')
                    print(f"  [OPEN] Port {port} ({service})")
        
        # Divide ports among threads
        ports = list(range(start_port, end_port + 1))
        chunk_size = len(ports) // threads
        thread_list = []
        
        for i in range(threads):
            start = i * chunk_size
            end = start + chunk_size if i < threads - 1 else len(ports)
            t = threading.Thread(target=worker, args=(ports[start:end],))
            thread_list.append(t)
            t.start()
        
        for t in thread_list:
            t.join()
        
        return sorted(self.open_ports)
    
    def detect_vulnerabilities(self) -> List[Dict]:
        """Detect vulnerabilities based on open ports"""
        self.vulnerabilities.clear()
        
        for port in self.open_ports:
            service = self.common_ports.get(port, None)
            
            if service in self.vulnerable_services:
                severity = self.vulnerable_services[service]
                
                vuln = {
                    'timestamp': datetime.now().isoformat(),
                    'port': port,
                    'service': service,
                    'severity': severity,
                    'description': f'{service} on port {port} is potentially vulnerable',
                    'recommendation': f'Disable {service} or restrict access'
                }
                
                self.vulnerabilities.append(vuln)
                
                # Update threat level
                if severity == 'CRITICAL':
                    self.threat_level = 'CRITICAL'
                elif severity == 'HIGH' and self.threat_level != 'CRITICAL':
                    self.threat_level = 'HIGH'
                elif severity == 'MEDIUM' and self.threat_level == 'LOW':
                    self.threat_level = 'MEDIUM'
        
        return list(self.vulnerabilities)
    
    def detect_intrusions(self, suspicious_ips: List[str]) -> List[Dict]:
        """Detect potential intrusions from suspicious IPs"""
        self.intrusions.clear()
        
        for ip in suspicious_ips:
            intrusion = {
                'timestamp': datetime.now().isoformat(),
                'ip': ip,
                'threat_level': 'MEDIUM',
                'reason': 'Suspicious connection pattern detected',
                'action': 'Investigate and block if necessary'
            }
            self.intrusions.append(intrusion)
        
        return list(self.intrusions)
    
    def generate_security_report(self, host: str) -> str:
        """Generate comprehensive security report"""
        report = f"""
╔════════════════════════════════════════════════════════╗
║         NETWORK SECURITY REPORT                        ║
║         Host: {host}
║         Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}                  ║
╚════════════════════════════════════════════════════════╝

THREAT LEVEL: {self.threat_level}

OPEN PORTS ({len(self.open_ports)}):
"""
        
        for port in sorted(self.open_ports):
            service = self.common_ports.get(port, 'Unknown')
            report += f"  {port:5d} - {service}\n"
        
        vulnerabilities = list(self.vulnerabilities)
        report += f"\nVULNERABILITIES DETECTED ({len(vulnerabilities)}):\n"
        
        for vuln in vulnerabilities:
            report += f"\n  [{vuln['severity']}] {vuln['service']} (Port {vuln['port']})\n"
            report += f"    Description: {vuln['description']}\n"
            report += f"    Recommendation: {vuln['recommendation']}\n"
        
        intrusions = list(self.intrusions)
        report += f"\nDETECTED INTRUSIONS ({len(intrusions)}):\n"
        
        for intrusion in intrusions:
            report += f"  {intrusion['ip']}: {intrusion['reason']}\n"
        
        report += "\nRECOMMENDATIONS:\n"
        if self.threat_level == 'CRITICAL':
            report += "  1. IMMEDIATE ACTION REQUIRED\n"
            report += "  2. Disable or restrict vulnerable services\n"
            report += "  3. Implement firewall rules\n"
            report += "  4. Monitor for intrusions\n"
        elif self.threat_level == 'HIGH':
            report += "  1. Review open ports and services\n"
            report += "  2. Update vulnerable software\n"
            report += "  3. Restrict network access\n"
        else:
            report += "  1. Continue monitoring\n"
            report += "  2. Maintain security patches\n"
        
        return report
    
    def export_report(self, filename: str):
        """Export security report to file"""
        data = {
            'timestamp': datetime.now().isoformat(),
            'threat_level': self.threat_level,
            'open_ports': self.open_ports,
            'vulnerabilities': list(self.vulnerabilities),
            'intrusions': list(self.intrusions)
        }
        
        with open(filename, 'w') as f:
            json.dump(data, f, indent=2)
        
        print(f"[SUCCESS] Security report exported to {filename}")

def main():
    print("""
    ╔════════════════════════════════════════════════════════╗
    ║  SecurityMonitor v2.0 - Advanced Python Edition        ║
    ║  Port Scanning • Vulnerability Detection • Intrusion   ║
    ╚════════════════════════════════════════════════════════╝
    """)
    
    monitor = SecurityMonitor()
    
    while True:
        print("\n[MENU]")
        print("1. Scan ports")
        print("2. Detect vulnerabilities")
        print("3. Detect intrusions")
        print("4. Generate security report")
        print("5. Exit")
        
        choice = input("\nSelect option: ").strip()
        
        if choice == "1":
            host = input("Target host (default 127.0.0.1): ").strip() or "127.0.0.1"
            end_port = int(input("End port (default 1024): ").strip() or "1024")
            print()
            monitor.scan_ports(host, end_port=end_port)
            print(f"\n[SUCCESS] Found {len(monitor.open_ports)} open ports")
        
        elif choice == "2":
            if not monitor.open_ports:
                print("[ERROR] No open ports detected. Scan first.")
                continue
            vulnerabilities = monitor.detect_vulnerabilities()
            print(f"\n[VULNERABILITIES] Found {len(vulnerabilities)} potential vulnerabilities")
            for vuln in vulnerabilities:
                print(f"  {vuln}")
        
        elif choice == "3":
            ips = input("Suspicious IPs (comma-separated): ").strip().split(',')
            ips = [ip.strip() for ip in ips if ip.strip()]
            intrusions = monitor.detect_intrusions(ips)
            print(f"\n[INTRUSIONS] Found {len(intrusions)} intrusions")
        
        elif choice == "4":
            host = input("Host (default 127.0.0.1): ").strip() or "127.0.0.1"
            print(monitor.generate_security_report(host))
            monitor.export_report('security_report.json')
        
        elif choice == "5":
            print("\nExiting...")
            break

if __name__ == "__main__":
    main()
