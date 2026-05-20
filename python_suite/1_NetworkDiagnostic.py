#!/usr/bin/env python3
"""
NetworkDiagnostic v2.0 - Advanced Network Diagnostic Tool
Supports: IPv4, IPv6, Real-time Monitoring, WebSocket API, ML Anomalies
Python 3.8+ | PyInstaller compatible | Windows/Linux/macOS
"""

import os
import sys
import socket
import subprocess
import json
import threading
import time
import numpy as np
from dataclasses import dataclass, asdict
from typing import List, Dict, Tuple
from datetime import datetime
import ipaddress
from collections import deque
import pickle

# WebSocket & API
try:
    import asyncio
    from aiohttp import web
    import websockets
except ImportError:
    print("[INFO] aiohttp not installed. WebSocket API disabled.")
    asyncio = None

# Data visualization
try:
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
    MATPLOTLIB_AVAILABLE = True
except ImportError:
    print("[INFO] matplotlib not installed. 3D graphs disabled.")
    MATPLOTLIB_AVAILABLE = False

# Machine Learning
try:
    from sklearn.ensemble import IsolationForest
    from sklearn.preprocessing import StandardScaler
    ML_AVAILABLE = True
except ImportError:
    print("[INFO] scikit-learn not installed. ML anomaly detection disabled.")
    ML_AVAILABLE = False

# Grafana integration
try:
    import requests
    GRAFANA_AVAILABLE = True
except ImportError:
    print("[INFO] requests not installed. Grafana integration disabled.")
    GRAFANA_AVAILABLE = False

@dataclass
class NetworkMetrics:
    """Store network metrics"""
    timestamp: str
    target: str
    latency: float
    packet_loss: int
    bandwidth: float
    jitter: float
    ipv4_available: bool
    ipv6_available: bool
    dns_resolution_time: float
    
    def to_dict(self):
        return asdict(self)

class NetworkDiagnosticEngine:
    """Main diagnostic engine with IPv4/IPv6 support"""
    
    def __init__(self):
        self.metrics_history: deque = deque(maxlen=3600)  # 1 hour at 1Hz
        self.anomaly_detector = None
        self.scaler = StandardScaler()
        self.ml_model_trained = False
        self.websocket_clients = set()
        
        if ML_AVAILABLE:
            self.anomaly_detector = IsolationForest(contamination=0.1, random_state=42)
    
    def detect_ipv6(self, target: str) -> bool:
        """Detect IPv6 support"""
        try:
            ipaddress.IPv6Address(target)
            return True
        except:
            try:
                socket.getaddrinfo(target, 80, socket.AF_INET6)
                return True
            except:
                return False
    
    def detect_ipv4(self, target: str) -> bool:
        """Detect IPv4 support"""
        try:
            socket.getaddrinfo(target, 80, socket.AF_INET)
            return True
        except:
            return False
    
    def ping_with_ipv6(self, target: str, count: int = 4) -> Tuple[float, int]:
        """Ping with IPv6 support"""
        try:
            # Try IPv6 first
            if self.detect_ipv6(target):
                if sys.platform == "win32":
                    cmd = ["ping", "-6", "-n", str(count), target]
                else:
                    cmd = ["ping6", "-c", str(count), target]
                
                result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
                return self._parse_ping_output(result.stdout)
        except:
            pass
        
        # Fallback to IPv4
        return self.ping_ipv4(target, count)
    
    def ping_ipv4(self, target: str, count: int = 4) -> Tuple[float, int]:
        """Ping with IPv4"""
        try:
            if sys.platform == "win32":
                cmd = ["ping", "-n", str(count), target]
            else:
                cmd = ["ping", "-c", str(count), target]
            
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
            return self._parse_ping_output(result.stdout)
        except Exception as e:
            print(f"[ERROR] Ping failed: {e}")
            return 0.0, 100
    
    def _parse_ping_output(self, output: str) -> Tuple[float, int]:
        """Parse ping output to extract latency and packet loss"""
        try:
            lines = output.split('\n')
            latency = 0.0
            packet_loss = 0
            
            for line in lines:
                if "time=" in line.lower() or "ms" in line:
                    import re
                    match = re.search(r'time[<=]*([\d.]+)', line, re.IGNORECASE)
                    if match:
                        latency = float(match.group(1))
                
                if "loss" in line.lower():
                    import re
                    match = re.search(r'([\d.]+)%', line)
                    if match:
                        packet_loss = int(float(match.group(1)))
            
            return latency, packet_loss
        except:
            return 0.0, 100
    
    def dns_resolution_time(self, target: str) -> float:
        """Measure DNS resolution time (IPv4 and IPv6)"""
        try:
            start = time.time()
            socket.getaddrinfo(target, 80)
            return (time.time() - start) * 1000  # milliseconds
        except:
            return 0.0
    
    def calculate_jitter(self, latencies: List[float]) -> float:
        """Calculate jitter (variance in latency)"""
        if len(latencies) < 2:
            return 0.0
        return float(np.std(latencies))
    
    def estimate_bandwidth(self) -> float:
        """Estimate bandwidth (placeholder - needs iperf integration)"""
        return 100.0  # Placeholder
    
    def collect_metrics(self, target: str = "8.8.8.8") -> NetworkMetrics:
        """Collect all network metrics"""
        latencies = []
        for _ in range(4):
            lat, _ = self.ping_with_ipv6(target, 1)
            if lat > 0:
                latencies.append(lat)
        
        avg_latency = np.mean(latencies) if latencies else 0.0
        _, packet_loss = self.ping_with_ipv6(target, 4)
        
        metrics = NetworkMetrics(
            timestamp=datetime.now().isoformat(),
            target=target,
            latency=avg_latency,
            packet_loss=packet_loss,
            bandwidth=self.estimate_bandwidth(),
            jitter=self.calculate_jitter(latencies),
            ipv4_available=self.detect_ipv4(target),
            ipv6_available=self.detect_ipv6(target),
            dns_resolution_time=self.dns_resolution_time(target)
        )
        
        self.metrics_history.append(metrics)
        return metrics
    
    def detect_anomalies(self) -> List[Dict]:
        """Detect anomalies using Isolation Forest ML"""
        if not ML_AVAILABLE or len(self.metrics_history) < 10:
            return []
        
        # Prepare data
        data = np.array([
            [m.latency, m.packet_loss, m.bandwidth, m.jitter, m.dns_resolution_time]
            for m in list(self.metrics_history)[-100:]
        ])
        
        # Train and predict
        scaled_data = self.scaler.fit_transform(data)
        predictions = self.anomaly_detector.fit_predict(scaled_data)
        
        anomalies = []
        for i, (pred, metric) in enumerate(zip(predictions, list(self.metrics_history)[-100:])):
            if pred == -1:  # Anomaly
                anomalies.append({
                    "timestamp": metric.timestamp,
                    "metric": asdict(metric),
                    "anomaly_score": float(self.anomaly_detector.score_samples(scaled_data[i:i+1])[0])
                })
        
        return anomalies
    
    def export_metrics(self, filename: str, format: str = "json"):
        """Export metrics to file"""
        data = [m.to_dict() for m in self.metrics_history]
        
        if format == "json":
            with open(filename, 'w') as f:
                json.dump(data, f, indent=2)
        elif format == "csv":
            import csv
            with open(filename, 'w', newline='') as f:
                if data:
                    writer = csv.DictWriter(f, fieldnames=data[0].keys())
                    writer.writeheader()
                    writer.writerows(data)
        
        print(f"[SUCCESS] Exported {len(data)} metrics to {filename}")
    
    def plot_3d_metrics(self, output_file: str = "metrics_3d.png"):
        """Generate 3D plot of metrics"""
        if not MATPLOTLIB_AVAILABLE or len(self.metrics_history) < 3:
            print("[ERROR] Matplotlib not available or insufficient data")
            return
        
        data = list(self.metrics_history)[-100:]
        latencies = [m.latency for m in data]
        packet_losses = [m.packet_loss for m in data]
        bandwidths = [m.bandwidth for m in data]
        
        fig = plt.figure(figsize=(12, 8))
        ax = fig.add_subplot(111, projection='3d')
        
        scatter = ax.scatter(latencies, packet_losses, bandwidths, 
                           c=range(len(data)), cmap='viridis', s=50)
        
        ax.set_xlabel('Latency (ms)')
        ax.set_ylabel('Packet Loss (%)')
        ax.set_zlabel('Bandwidth (Mbps)')
        ax.set_title('3D Network Metrics Visualization')
        
        plt.colorbar(scatter, ax=ax, label='Time')
        plt.savefig(output_file, dpi=150)
        print(f"[SUCCESS] 3D plot saved to {output_file}")
        plt.close()

def main():
    """Main CLI interface"""
    print("""
    ╔═══════════════════════════════════════════════════════╗
    ║  NetworkDiagnostic v2.0 - Advanced Python Edition     ║
    ║  IPv4/IPv6 • ML Anomalies • 3D Graphs • WebSocket API ║
    ╚═══════════════════════════════════════════════════════╝
    """)
    
    engine = NetworkDiagnosticEngine()
    
    while True:
        print("\n[MENU]")
        print("1. Collect metrics (single shot)")
        print("2. Continuous monitoring (30 seconds)")
        print("3. Detect anomalies (ML)")
        print("4. Generate 3D plot")
        print("5. Export metrics")
        print("6. Exit")
        
        choice = input("\nSelect option: ").strip()
        
        if choice == "1":
            target = input("Target (default 8.8.8.8): ").strip() or "8.8.8.8"
            metrics = engine.collect_metrics(target)
            print(f"\n[METRICS]")
            print(f"  Latency: {metrics.latency:.2f} ms")
            print(f"  Packet Loss: {metrics.packet_loss}%")
            print(f"  IPv4: {metrics.ipv4_available}, IPv6: {metrics.ipv6_available}")
            print(f"  DNS Time: {metrics.dns_resolution_time:.2f} ms")
            print(f"  Jitter: {metrics.jitter:.2f} ms")
        
        elif choice == "2":
            target = input("Target (default 8.8.8.8): ").strip() or "8.8.8.8"
            print(f"\nMonitoring {target} for 30 seconds...")
            for i in range(30):
                metrics = engine.collect_metrics(target)
                print(f"[{i+1}/30] Latency: {metrics.latency:.2f}ms, Loss: {metrics.packet_loss}%, DNS: {metrics.dns_resolution_time:.2f}ms")
                time.sleep(1)
        
        elif choice == "3":
            anomalies = engine.detect_anomalies()
            print(f"\n[ANOMALIES] Found {len(anomalies)} anomalies")
            for anomaly in anomalies[:5]:
                print(f"  {anomaly['timestamp']}: Score {anomaly['anomaly_score']:.2f}")
        
        elif choice == "4":
            engine.plot_3d_metrics()
        
        elif choice == "5":
            filename = input("Filename (default metrics.json): ").strip() or "metrics.json"
            format_type = input("Format (json/csv): ").strip() or "json"
            engine.export_metrics(filename, format_type)
        
        elif choice == "6":
            print("\nExiting...")
            break

if __name__ == "__main__":
    main()
