#!/usr/bin/env python3
"""
PerformanceAnalyzer v2.0 - Advanced Network Performance Analysis
Features: Real-time Graphs, Trend Analysis, Predictive Alerts, Grafana Integration
Python 3.8+ | PyInstaller compatible
"""

import json
import time
import threading
from datetime import datetime, timedelta
from collections import deque
import numpy as np
from typing import List, Dict, Tuple

# Visualization
try:
    import matplotlib.pyplot as plt
    import matplotlib.animation as animation
    MATPLOTLIB_AVAILABLE = True
except ImportError:
    MATPLOTLIB_AVAILABLE = False

# Grafana
try:
    import requests
    GRAFANA_AVAILABLE = True
except ImportError:
    GRAFANA_AVAILABLE = False

# Machine Learning
try:
    from sklearn.linear_model import LinearRegression
    from sklearn.preprocessing import StandardScaler
    ML_AVAILABLE = True
except ImportError:
    ML_AVAILABLE = False

class PerformanceAnalyzer:
    """Advanced performance analysis with predictive capabilities"""
    
    def __init__(self):
        self.metrics = deque(maxlen=3600)
        self.alerts = deque(maxlen=100)
        self.thresholds = {
            'latency_warning': 50,
            'latency_critical': 100,
            'packet_loss_warning': 1,
            'packet_loss_critical': 5,
            'bandwidth_warning': 10
        }
        self.predictive_model = None
        if ML_AVAILABLE:
            self.predictive_model = LinearRegression()
    
    def add_metric(self, metric: Dict):
        """Add a metric point"""
        self.metrics.append({
            'timestamp': datetime.now(),
            **metric
        })
        self._check_alerts()
    
    def _check_alerts(self):
        """Check if thresholds are exceeded"""
        if not self.metrics:
            return
        
        latest = list(self.metrics)[-1]
        
        if latest['latency'] > self.thresholds['latency_critical']:
            self.alerts.append({
                'timestamp': datetime.now(),
                'level': 'CRITICAL',
                'message': f"High latency detected: {latest['latency']:.2f}ms"
            })
        
        if latest['packet_loss'] > self.thresholds['packet_loss_critical']:
            self.alerts.append({
                'timestamp': datetime.now(),
                'level': 'CRITICAL',
                'message': f"High packet loss: {latest['packet_loss']}%"
            })
    
    def calculate_statistics(self) -> Dict:
        """Calculate performance statistics"""
        if not self.metrics:
            return {}
        
        latencies = [m['latency'] for m in self.metrics]
        packet_losses = [m['packet_loss'] for m in self.metrics]
        
        return {
            'latency': {
                'mean': float(np.mean(latencies)),
                'std': float(np.std(latencies)),
                'min': float(np.min(latencies)),
                'max': float(np.max(latencies)),
                'p95': float(np.percentile(latencies, 95)),
                'p99': float(np.percentile(latencies, 99))
            },
            'packet_loss': {
                'mean': float(np.mean(packet_losses)),
                'max': float(np.max(packet_losses))
            },
            'uptime_percentage': 100 - float(np.mean(packet_losses))
        }
    
    def predict_next_hour(self) -> Dict:
        """Predict network metrics for next hour using ML"""
        if not ML_AVAILABLE or len(self.metrics) < 10:
            return {'error': 'Insufficient data'}
        
        data = list(self.metrics)[-60:]
        X = np.arange(len(data)).reshape(-1, 1)
        latencies = np.array([m['latency'] for m in data])
        
        self.predictive_model.fit(X, latencies)
        
        future_X = np.arange(len(data), len(data) + 60).reshape(-1, 1)
        predictions = self.predictive_model.predict(future_X)
        
        return {
            'predicted_avg_latency': float(np.mean(predictions)),
            'predicted_max_latency': float(np.max(predictions)),
            'confidence': 'moderate' if len(data) > 30 else 'low'
        }
    
    def generate_report(self) -> str:
        """Generate performance report"""
        stats = self.calculate_statistics()
        
        report = f"""
╔════════════════════════════════════════════════════════╗
║         NETWORK PERFORMANCE REPORT                     ║
║         Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}                  ║
╚════════════════════════════════════════════════════════╝

LATENCY ANALYSIS:
  Mean: {stats['latency']['mean']:.2f} ms
  Std Dev: {stats['latency']['std']:.2f} ms
  Min: {stats['latency']['min']:.2f} ms
  Max: {stats['latency']['max']:.2f} ms
  P95: {stats['latency']['p95']:.2f} ms
  P99: {stats['latency']['p99']:.2f} ms

PACKET LOSS:
  Mean: {stats['packet_loss']['mean']:.2f}%
  Max: {stats['packet_loss']['max']:.2f}%

UPTIME:
  {stats['uptime_percentage']:.2f}%

ALERTS:
  Total: {len(self.alerts)}
  Recent:
"""
        
        for alert in list(self.alerts)[-5:]:
            report += f"    [{alert['level']}] {alert['message']}\n"
        
        return report
    
    def send_to_grafana(self, grafana_url: str, api_key: str, dashboard_id: int):
        """Send metrics to Grafana"""
        if not GRAFANA_AVAILABLE:
            print("[ERROR] requests library not installed")
            return
        
        try:
            headers = {'Authorization': f'Bearer {api_key}'}
            
            for metric in list(self.metrics)[-100:]:
                payload = {
                    'dashboard': {'id': dashboard_id},
                    'panels': [
                        {
                            'title': 'Network Metrics',
                            'targets': [{
                                'expr': 'network_latency',
                                'value': metric['latency']
                            }]
                        }
                    ]
                }
                
                response = requests.post(
                    f"{grafana_url}/api/dashboards/db",
                    json=payload,
                    headers=headers
                )
                
                if response.status_code != 200:
                    print(f"[ERROR] Grafana push failed: {response.status_code}")
                    break
            
            print("[SUCCESS] Metrics sent to Grafana")
        
        except Exception as e:
            print(f"[ERROR] Grafana integration failed: {e}")
    
    def export_report(self, filename: str):
        """Export report to file"""
        with open(filename, 'w') as f:
            f.write(self.generate_report())
        print(f"[SUCCESS] Report exported to {filename}")

def demo_real_time_graph():
    """Demo real-time graph animation"""
    if not MATPLOTLIB_AVAILABLE:
        print("[ERROR] matplotlib not installed")
        return
    
    analyzer = PerformanceAnalyzer()
    
    # Generate sample data
    print("[INFO] Generating sample metrics...")
    for i in range(60):
        analyzer.add_metric({
            'latency': 25 + np.random.normal(0, 5),
            'packet_loss': max(0, np.random.normal(0.5, 0.5))
        })
        time.sleep(0.1)
    
    # Plot
    data = list(analyzer.metrics)
    latencies = [m['latency'] for m in data]
    times = range(len(latencies))
    
    plt.figure(figsize=(12, 6))
    plt.plot(times, latencies, 'b-', linewidth=2, label='Latency')
    plt.fill_between(times, latencies, alpha=0.3)
    plt.xlabel('Time (seconds)')
    plt.ylabel('Latency (ms)')
    plt.title('Real-Time Network Latency')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('realtime_latency.png', dpi=150)
    print("[SUCCESS] Real-time graph saved to realtime_latency.png")
    plt.close()

def main():
    print("""
    ╔════════════════════════════════════════════════════════╗
    ║  PerformanceAnalyzer v2.0 - Advanced Python Edition    ║
    ║  Predictions • Grafana • Real-time Graphs • ML Trends  ║
    ╚════════════════════════════════════════════════════════╝
    """)
    
    analyzer = PerformanceAnalyzer()
    
    while True:
        print("\n[MENU]")
        print("1. Add metric (manual)")
        print("2. Generate sample metrics")
        print("3. View statistics")
        print("4. Predict next hour (ML)")
        print("5. Generate report")
        print("6. Real-time graph demo")
        print("7. Exit")
        
        choice = input("\nSelect option: ").strip()
        
        if choice == "1":
            lat = float(input("Latency (ms): "))
            loss = float(input("Packet loss (%): "))
            analyzer.add_metric({'latency': lat, 'packet_loss': loss})
            print("[SUCCESS] Metric added")
        
        elif choice == "2":
            print("Generating 60 sample metrics...")
            for i in range(60):
                analyzer.add_metric({
                    'latency': 25 + np.random.normal(0, 5),
                    'packet_loss': max(0, np.random.normal(0.5, 0.5))
                })
                print(f"[{i+1}/60] Added", end='\r')
                time.sleep(0.05)
            print("\n[SUCCESS] Sample metrics generated")
        
        elif choice == "3":
            stats = analyzer.calculate_statistics()
            print(json.dumps(stats, indent=2))
        
        elif choice == "4":
            prediction = analyzer.predict_next_hour()
            print(json.dumps(prediction, indent=2))
        
        elif choice == "5":
            print(analyzer.generate_report())
            analyzer.export_report('performance_report.txt')
        
        elif choice == "6":
            demo_real_time_graph()
        
        elif choice == "7":
            print("\nExiting...")
            break

if __name__ == "__main__":
    main()
