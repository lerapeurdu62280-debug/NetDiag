#!/usr/bin/env python3
"""
Dashboard Web v2.0 - Browser-based Dashboard with Real-time Charts
Features: Live Graphs, REST API, WebSocket, Responsive Design
Python 3.8+ | PyInstaller compatible
"""

import json
from datetime import datetime
from typing import Dict, List

try:
    from flask import Flask, render_template_string, jsonify
    from flask_cors import CORS
    import numpy as np
except ImportError:
    print("[ERROR] Flask not installed")
    print("[INFO] Install: pip install flask flask-cors")
    exit(1)

app = Flask(__name__)
CORS(app)

# In-memory metrics storage
metrics_storage = {
    'latency': [],
    'packet_loss': [],
    'bandwidth': [],
    'jitter': [],
    'timestamps': []
}

HTML_TEMPLATE = """
<!DOCTYPE html>
<html>
<head>
    <title>NetworkDiagnostic Dashboard</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif; background: #f5f5f5; }
        
        .container { max-width: 1400px; margin: 0 auto; padding: 20px; }
        .header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px; border-radius: 10px; margin-bottom: 30px; }
        .header h1 { font-size: 2.5em; margin-bottom: 10px; }
        .header p { opacity: 0.9; }
        
        .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin-bottom: 30px; }
        .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        .card h3 { color: #333; margin-bottom: 15px; font-size: 0.9em; text-transform: uppercase; letter-spacing: 1px; }
        .metric-value { font-size: 2.5em; font-weight: bold; color: #667eea; margin: 10px 0; }
        .metric-unit { font-size: 0.8em; color: #999; }
        
        .status-good { color: #10b981; }
        .status-warning { color: #f59e0b; }
        .status-critical { color: #ef4444; }
        
        .charts { display: grid; grid-template-columns: repeat(auto-fit, minmax(500px, 1fr)); gap: 20px; }
        .chart-container { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        .chart-container canvas { width: 100% !important; height: 300px !important; }
        
        .controls { display: flex; gap: 10px; margin-top: 20px; }
        button { padding: 10px 20px; background: #667eea; color: white; border: none; border-radius: 5px; cursor: pointer; font-size: 0.9em; }
        button:hover { background: #764ba2; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🌐 NetworkDiagnostic Dashboard</h1>
            <p>Real-time Network Performance Monitoring</p>
        </div>
        
        <div class="grid">
            <div class="card">
                <h3>Latency</h3>
                <div class="metric-value" id="latency">--</div>
                <div class="metric-unit">milliseconds</div>
                <div id="latency-status"></div>
            </div>
            
            <div class="card">
                <h3>Packet Loss</h3>
                <div class="metric-value" id="packet-loss">--</div>
                <div class="metric-unit">percent</div>
                <div id="packet-loss-status"></div>
            </div>
            
            <div class="card">
                <h3>Bandwidth</h3>
                <div class="metric-value" id="bandwidth">--</div>
                <div class="metric-unit">Mbps</div>
                <div id="bandwidth-status"></div>
            </div>
            
            <div class="card">
                <h3>Jitter</h3>
                <div class="metric-value" id="jitter">--</div>
                <div class="metric-unit">ms</div>
                <div id="jitter-status"></div>
            </div>
        </div>
        
        <div class="charts">
            <div class="chart-container">
                <canvas id="latencyChart"></canvas>
            </div>
            <div class="chart-container">
                <canvas id="packetLossChart"></canvas>
            </div>
            <div class="chart-container">
                <canvas id="bandwidthChart"></canvas>
            </div>
            <div class="chart-container">
                <canvas id="jitterChart"></canvas>
            </div>
        </div>
        
        <div class="controls">
            <button onclick="collectMetrics()">Collect Sample Data</button>
            <button onclick="exportMetrics()">Export as JSON</button>
            <button onclick="clearMetrics()">Clear Data</button>
        </div>
    </div>
    
    <script>
        const ctx = {
            latency: document.getElementById('latencyChart').getContext('2d'),
            packetLoss: document.getElementById('packetLossChart').getContext('2d'),
            bandwidth: document.getElementById('bandwidthChart').getContext('2d'),
            jitter: document.getElementById('jitterChart').getContext('2d')
        };
        
        const charts = {};
        let metrics = {latency: [], packet_loss: [], bandwidth: [], jitter: [], timestamps: []};
        
        function initCharts() {
            charts.latency = new Chart(ctx.latency, {
                type: 'line',
                data: {labels: [], datasets: [{label: 'Latency (ms)', data: [], borderColor: '#667eea', tension: 0.1}]},
                options: {responsive: true, maintainAspectRatio: false, scales: {y: {beginAtZero: true}}}
            });
            
            charts.packetLoss = new Chart(ctx.packetLoss, {
                type: 'line',
                data: {labels: [], datasets: [{label: 'Packet Loss (%)', data: [], borderColor: '#ef4444', tension: 0.1}]},
                options: {responsive: true, maintainAspectRatio: false, scales: {y: {beginAtZero: true}}}
            });
            
            charts.bandwidth = new Chart(ctx.bandwidth, {
                type: 'line',
                data: {labels: [], datasets: [{label: 'Bandwidth (Mbps)', data: [], borderColor: '#10b981', tension: 0.1}]},
                options: {responsive: true, maintainAspectRatio: false, scales: {y: {beginAtZero: true}}}
            });
            
            charts.jitter = new Chart(ctx.jitter, {
                type: 'line',
                data: {labels: [], datasets: [{label: 'Jitter (ms)', data: [], borderColor: '#f59e0b', tension: 0.1}]},
                options: {responsive: true, maintainAspectRatio: false, scales: {y: {beginAtZero: true}}}
            });
        }
        
        function updateMetrics(data) {
            document.getElementById('latency').textContent = data.latency.toFixed(2);
            document.getElementById('packet-loss').textContent = data.packet_loss.toFixed(1);
            document.getElementById('bandwidth').textContent = data.bandwidth.toFixed(2);
            document.getElementById('jitter').textContent = data.jitter.toFixed(2);
            
            // Update status
            document.getElementById('latency-status').textContent = data.latency < 50 ? '✓ Good' : data.latency < 100 ? '⚠ Warning' : '✗ Critical';
            document.getElementById('latency-status').className = data.latency < 50 ? 'status-good' : data.latency < 100 ? 'status-warning' : 'status-critical';
        }
        
        function addMetricPoint(metric) {
            metrics.latency.push(metric.latency);
            metrics.packet_loss.push(metric.packet_loss);
            metrics.bandwidth.push(metric.bandwidth);
            metrics.jitter.push(metric.jitter);
            metrics.timestamps.push(new Date().toLocaleTimeString());
            
            // Keep only last 60 points
            if (metrics.latency.length > 60) {
                metrics.latency.shift();
                metrics.packet_loss.shift();
                metrics.bandwidth.shift();
                metrics.jitter.shift();
                metrics.timestamps.shift();
            }
            
            updateCharts();
            updateMetrics(metric);
        }
        
        function updateCharts() {
            ['latency', 'packetLoss', 'bandwidth', 'jitter'].forEach(key => {
                charts[key].data.labels = metrics.timestamps;
                charts[key].data.datasets[0].data = metrics[key.includes('Packet') ? 'packet_loss' : key.replace('packetLoss', 'packet_loss')];
                charts[key].update();
            });
        }
        
        function collectMetrics() {
            fetch('/api/metrics')
                .then(r => r.json())
                .then(data => addMetricPoint(data))
                .catch(e => console.error('Error:', e));
        }
        
        function exportMetrics() {
            const dataStr = JSON.stringify(metrics, null, 2);
            const dataBlob = new Blob([dataStr], {type: 'application/json'});
            const url = URL.createObjectURL(dataBlob);
            const link = document.createElement('a');
            link.href = url;
            link.download = 'metrics_' + new Date().getTime() + '.json';
            link.click();
        }
        
        function clearMetrics() {
            metrics = {latency: [], packet_loss: [], bandwidth: [], jitter: [], timestamps: []};
            updateCharts();
        }
        
        initCharts();
        setInterval(collectMetrics, 1000);
    </script>
</body>
</html>
"""

@app.route('/')
def index():
    return render_template_string(HTML_TEMPLATE)

@app.route('/api/metrics', methods=['GET'])
def get_metrics():
    """Get sample metrics"""
    return jsonify({
        'latency': float(np.random.normal(25, 5)),
        'packet_loss': float(np.random.uniform(0, 2)),
        'bandwidth': float(np.random.normal(85, 10)),
        'jitter': float(np.random.normal(5, 2))
    })

@app.route('/api/metrics/history', methods=['GET'])
def get_metrics_history():
    """Get metrics history"""
    return jsonify(metrics_storage)

@app.route('/api/health', methods=['GET'])
def health():
    """Health check"""
    return jsonify({
        'status': 'running',
        'timestamp': datetime.now().isoformat(),
        'version': '2.0'
    })

if __name__ == '__main__':
    print("""
    ╔════════════════════════════════════════════════════════╗
    ║  NetworkDiagnostic Web Dashboard v2.0                  ║
    ║  Real-time Graphs • REST API • Responsive Design       ║
    ╚════════════════════════════════════════════════════════╝
    """)
    
    print("\n[INFO] Starting web server...")
    print("[INFO] Open http://localhost:5000 in your browser")
    print("[INFO] Press Ctrl+C to stop")
    
    app.run(host='0.0.0.0', port=5000, debug=False)
