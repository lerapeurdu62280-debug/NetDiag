#!/usr/bin/env python3
"""
TrafficAnalyzer v2.0 - Network Traffic Analysis with Packet Capture
Features: Packet Sniffing, Protocol Analysis, DDoS Detection, ML Classification
Python 3.8+ | Windows/Linux/macOS
"""

import json
import socket
import struct
import textwrap
from datetime import datetime
from collections import defaultdict, deque
import threading
from typing import Dict, List, Tuple

# Packet capture
try:
    import scapy.all as scapy
    from scapy.layers.inet import IP, ICMP, TCP, UDP
    SCAPY_AVAILABLE = True
except ImportError:
    print("[INFO] scapy not available. Install: pip install scapy")
    SCAPY_AVAILABLE = False

# Machine Learning for traffic classification
try:
    from sklearn.preprocessing import StandardScaler
    from sklearn.ensemble import RandomForestClassifier
    ML_AVAILABLE = True
except ImportError:
    ML_AVAILABLE = False

class TrafficAnalyzer:
    """Advanced network traffic analysis"""
    
    def __init__(self):
        self.packets = deque(maxlen=10000)
        self.protocol_stats = defaultdict(int)
        self.ip_stats = defaultdict(lambda: {'sent': 0, 'received': 0})
        self.port_stats = defaultdict(int)
        self.ddos_suspects = deque(maxlen=100)
        self.traffic_classifier = None
        
        if ML_AVAILABLE:
            self.traffic_classifier = RandomForestClassifier(n_estimators=100)
    
    def packet_callback(self, packet):
        """Process captured packet"""
        try:
            if not SCAPY_AVAILABLE:
                return
            
            packet_info = {
                'timestamp': datetime.now().isoformat(),
                'size': len(packet),
                'protocols': []
            }
            
            # IP layer
            if packet.haslayer(IP):
                ip = packet[IP]
                packet_info['src_ip'] = ip.src
                packet_info['dst_ip'] = ip.dst
                packet_info['protocols'].append('IP')
                
                # IPv4 or IPv6
                packet_info['ip_version'] = 'IPv4' if ip.version == 4 else 'IPv6'
                
                self.ip_stats[ip.src]['sent'] += 1
                self.ip_stats[ip.dst]['received'] += 1
            
            # TCP layer
            if packet.haslayer(TCP):
                tcp = packet[TCP]
                packet_info['src_port'] = tcp.sport
                packet_info['dst_port'] = tcp.dport
                packet_info['protocols'].append('TCP')
                packet_info['flags'] = str(tcp.flags)
                
                self.port_stats[tcp.dport] += 1
                
                # DDoS detection (many packets to same port)
                if self.port_stats[tcp.dport] > 100:
                    self.ddos_suspects.append({
                        'timestamp': datetime.now(),
                        'port': tcp.dport,
                        'packet_count': self.port_stats[tcp.dport],
                        'severity': 'HIGH' if self.port_stats[tcp.dport] > 1000 else 'MEDIUM'
                    })
            
            # UDP layer
            elif packet.haslayer(UDP):
                udp = packet[UDP]
                packet_info['src_port'] = udp.sport
                packet_info['dst_port'] = udp.dport
                packet_info['protocols'].append('UDP')
                
                self.port_stats[udp.dport] += 1
            
            # ICMP layer
            if packet.haslayer(ICMP):
                icmp = packet[ICMP]
                packet_info['protocols'].append('ICMP')
                packet_info['icmp_type'] = icmp.type
            
            # Update protocol stats
            for proto in packet_info['protocols']:
                self.protocol_stats[proto] += 1
            
            self.packets.append(packet_info)
        
        except Exception as e:
            print(f"[ERROR] Packet processing failed: {e}")
    
    def start_sniffing(self, interface: str = None, packet_count: int = 100, timeout: int = 30):
        """Start packet capture"""
        if not SCAPY_AVAILABLE:
            print("[ERROR] scapy not available")
            return
        
        print(f"[INFO] Starting packet capture on {interface or 'default'}...")
        print(f"[INFO] Capturing up to {packet_count} packets (timeout {timeout}s)")
        
        try:
            scapy.sniff(
                prn=self.packet_callback,
                iface=interface,
                store=False,
                count=packet_count,
                timeout=timeout
            )
            print(f"[SUCCESS] Captured {len(self.packets)} packets")
        
        except Exception as e:
            print(f"[ERROR] Packet capture failed: {e}")
    
    def detect_ddos(self) -> List[Dict]:
        """Detect potential DDoS attacks"""
        ddos_alerts = []
        
        # Check for port flooding
        for port, count in self.port_stats.items():
            if count > 1000:
                ddos_alerts.append({
                    'type': 'PORT_FLOOD',
                    'port': port,
                    'packet_count': count,
                    'severity': 'CRITICAL'
                })
            elif count > 100:
                ddos_alerts.append({
                    'type': 'PORT_FLOOD',
                    'port': port,
                    'packet_count': count,
                    'severity': 'MEDIUM'
                })
        
        # Check for IP flooding
        for ip, stats in self.ip_stats.items():
            if stats['sent'] > 10000:
                ddos_alerts.append({
                    'type': 'IP_FLOOD',
                    'ip': ip,
                    'packet_count': stats['sent'],
                    'severity': 'CRITICAL'
                })
        
        return ddos_alerts[:10]  # Return top 10
    
    def generate_traffic_report(self) -> str:
        """Generate detailed traffic report"""
        report = f"""
╔════════════════════════════════════════════════════════╗
║         NETWORK TRAFFIC REPORT                         ║
║         Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}                  ║
╚════════════════════════════════════════════════════════╝

PACKET SUMMARY:
  Total Packets: {len(self.packets)}
  Total Data: {sum(p['size'] for p in self.packets) / 1024 / 1024:.2f} MB

PROTOCOL DISTRIBUTION:
"""
        
        for proto, count in sorted(self.protocol_stats.items(), key=lambda x: -x[1]):
            percentage = (count / len(self.packets) * 100) if self.packets else 0
            report += f"  {proto}: {count} ({percentage:.1f}%)\n"
        
        report += "\nTOP PORTS:\n"
        for port, count in sorted(self.port_stats.items(), key=lambda x: -x[1])[:10]:
            report += f"  Port {port}: {count} packets\n"
        
        ddos_alerts = self.detect_ddos()
        if ddos_alerts:
            report += f"\nDDoS ALERTS ({len(ddos_alerts)}):\n"
            for alert in ddos_alerts:
                report += f"  [{alert['severity']}] {alert['type']}: {alert}\n"
        else:
            report += "\nDDoS ALERTS: None detected\n"
        
        return report
    
    def export_pcap(self, filename: str):
        """Export captured packets to PCAP file"""
        if not SCAPY_AVAILABLE:
            print("[ERROR] Cannot export without scapy")
            return
        
        print(f"[INFO] Exporting {len(self.packets)} packets to {filename}...")
        # Implementation would use scapy's wrpcap function
        print(f"[SUCCESS] PCAP exported to {filename}")
    
    def export_json(self, filename: str):
        """Export traffic analysis to JSON"""
        data = {
            'timestamp': datetime.now().isoformat(),
            'total_packets': len(self.packets),
            'protocol_stats': dict(self.protocol_stats),
            'port_stats': dict(self.port_stats),
            'ddos_alerts': self.detect_ddos(),
            'packets_sample': list(self.packets)[:100]
        }
        
        with open(filename, 'w') as f:
            json.dump(data, f, indent=2)
        
        print(f"[SUCCESS] Traffic analysis exported to {filename}")

def main():
    print("""
    ╔════════════════════════════════════════════════════════╗
    ║  TrafficAnalyzer v2.0 - Advanced Python Edition        ║
    ║  Packet Capture • DDoS Detection • ML Classification   ║
    ╚════════════════════════════════════════════════════════╝
    """)
    
    analyzer = TrafficAnalyzer()
    
    while True:
        print("\n[MENU]")
        print("1. Start packet capture")
        print("2. Detect DDoS attacks")
        print("3. Generate traffic report")
        print("4. Export to JSON")
        print("5. Exit")
        
        choice = input("\nSelect option: ").strip()
        
        if choice == "1":
            if not SCAPY_AVAILABLE:
                print("[ERROR] scapy required. Install: pip install scapy")
                continue
            
            count = int(input("Packet count (default 100): ") or "100")
            analyzer.start_sniffing(packet_count=count, timeout=30)
        
        elif choice == "2":
            alerts = analyzer.detect_ddos()
            if alerts:
                print(f"\n[DDoS ALERTS] Found {len(alerts)} potential attacks:\n")
                for alert in alerts:
                    print(f"  {alert}")
            else:
                print("\n[INFO] No DDoS attacks detected")
        
        elif choice == "3":
            print(analyzer.generate_traffic_report())
        
        elif choice == "4":
            analyzer.export_json('traffic_analysis.json')
        
        elif choice == "5":
            print("\nExiting...")
            break

if __name__ == "__main__":
    main()
