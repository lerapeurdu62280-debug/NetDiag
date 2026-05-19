# NetDiag PRO - Advanced Network Diagnostic Tool

## 🎉 Version Complète avec Toutes les Améliorations

NetDiag PRO est la version **ultime et professionnelle** d'un outil de diagnostic réseau pour Windows avec des fonctionnalités avancées.

---

## 📦 Nouvelles Fonctionnalités

### ✅ **Interface GUI avec Qt** 
- Interface graphique moderne et intuitive
- Affichage en temps réel des métriques
- Graphiques interactifs (Bandwidth, Latency)
- Tableau de bord complet

### ✅ **Export de Résultats (CSV/JSON)**
- Export des tests en CSV pour Excel
- Export structuré en JSON
- Format compatible avec les outils d'analyse
- Horodatage automatique

### ✅ **Monitoring en Temps Réel**
- Surveillance continue du réseau (1-60 secondes)
- Capture de:
  - Bande passante (Mbps)
  - Latence (ms)
  - Perte de paquets (%)
  - Connexions actives
  - Utilisation CPU/Mémoire
- Historique métriques (jusqu'à 3600 points)
- Thread-safe avec mutex

### ✅ **Graphiques de Performance**
- Graphiques spline interactifs
- Mise à jour en temps réel
- Zoom et pan
- Export graphiques (PNG/SVG)
- Multi-séries (Bandwidth, Latency, etc)

### ✅ **Capture de Paquets**
- Capture jusqu'à 100+ paquets
- Extraction source/destination IP
- Port source/destination
- Type de protocole (TCP/UDP/ICMP)
- Taille et timestamp
- Export capture en PCAP (optionnel)

### ✅ **Analyse de Performance**
- Rapport détaillé de santé réseau
- Moyennes, max, min des métriques
- Classement: GOOD/FAIR/POOR
- Recommandations automatiques
- Export rapport texte

---

## 🏗️ Architecture Avancée

```
NetDiag_Pro/
├── src/
│   ├── MonitoringEngine.h/cpp      # Monitoring temps réel
│   ├── DataExporter.h/cpp          # Export CSV/JSON
│   ├── PacketCapture.h/cpp         # Capture paquets
│   ├── PerformanceAnalyzer.h/cpp   # Analyse performance
│   ├── main_cli.cpp                # Interface CLI avancée
│   └── [fichiers standards]
├── gui/
│   ├── MainWindow.h/cpp            # GUI principale
│   ├── main_gui.cpp                # Entrée GUI
│   └── [autres composants]
├── CMakeLists_Advanced.txt
└── README_ADVANCED.md
```

---

## 🚀 Installation Avancée

### Prérequis
- Windows 10+ ou Windows Server 2016+
- Visual Studio 2019+ avec C++17
- CMake 3.16+
- Qt 5.15+ (pour GUI)
- Python 3.8+ (optionnel, pour scripts)

### Compilation CLI

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release --target NetDiag_CLI
cd bin
.\NetDiag_CLI.exe
```

### Compilation GUI

```bash
mkdir build_gui
cd build_gui
cmake .. -G "Visual Studio 16 2019" -DCMAKE_PREFIX_PATH="C:/Qt/5.15.2/msvc2019_64"
cmake --build . --config Release --target NetDiag_GUI
cd bin
.\NetDiag_GUI.exe
```

### Compilation complète (CLI + GUI)

```bash
mkdir build
cd build
cmake -f ../CMakeLists_Advanced.txt
cmake --build . --config Release
```

---

## 🎮 Utilisation

### Mode CLI - Monitoring en Temps Réel

```
7. Real-Time Monitoring
Enter monitoring duration (seconds): 30
Monitoring for 30 seconds...

[Affiche tableau avec mise à jour chaque seconde]
Timestamp            | Bandwidth | Latency | Packet Loss | Connections
2024-01-15 10:30:45 | 85.50     | 25.3    | 0           | 42
2024-01-15 10:30:46 | 87.20     | 24.8    | 0           | 41
...

Export monitoring data? (csv/json/none): csv
→ Data exported to monitoring_data.csv
```

### Mode CLI - Packet Capture

```
8. Packet Capture
Number of packets to capture: 100

[Affiche tableau paquets]
Timestamp            | Source IP     | Dest IP      | Src Port | Dst Port | Protocol | Length
2024-01-15 10:30:45 | 192.168.1.100 | 8.8.8.8      | 55234    | 443      | TCP      | 1460
2024-01-15 10:30:46 | 192.168.1.100 | 208.67.222.1 | 55235    | 53       | UDP      | 512
...

Total packets captured: 100
```

### Mode CLI - Performance Analysis

```
9. Performance Analysis

=== NETWORK PERFORMANCE REPORT ===

Overall Status: GOOD

Bandwidth Analysis:
  Average: 85.50 Mbps
  Maximum: 92.30 Mbps
  Minimum: 78.40 Mbps

Latency Analysis:
  Average: 25.30 ms
  Maximum: 35.20 ms
  Minimum: 18.50 ms

Packet Loss:
  Average: 0%
  Maximum: 1%

Recommendations:
  - Network performance is optimal. No issues detected.

Export report? (txt/none): txt
→ Report exported to performance_report.txt
```

### Mode GUI - Interface Graphique

```
Fenêtre GUI:
┌─────────────────────────────────────┬──────────────────────┐
│  Network Tests          │ Real-Time Metrics Chart │
│ ─────────────────────  │                          │
│ ✓ Network Information   │                          │
│ ✓ Ping Test             │   [Graphique Spline]    │
│ ✓ DNS Lookup            │   Bandwidth: ▄▄▄▄▃      │
│ ✓ Start Monitoring      │   Latency:   ▂▂▂▃▃▄     │
│ ✓ Packet Capture        │                          │
│ ✓ Export Results        │                          │
│                         │                          │
│ Current Status:         │                          │
│ ─────────────────────  │                          │
│ Bandwidth: 85.50 Mbps   │                          │
│ Latency: 25.30 ms       │                          │
│ Packet Loss: 0%         │                          │
│ Status: Ready           │                          │
└─────────────────────────────────────┴──────────────────────┘
```

---

## 📊 Formats d'Export

### CSV Export Exemple

```csv
Target,Packets Sent,Packets Received,Packet Loss (%),Min Time (ms),Max Time (ms),Avg Time (ms),Status
google.com,4,4,0,22.5,28.3,25.4,Success
```

### JSON Export Exemple

```json
{
  "exportTime": "2024-01-15 10:30:45",
  "testType": "monitoring",
  "dataPoints": 30,
  "data": [
    {
      "timestamp": "2024-01-15 10:30:45",
      "bandwidth": 85.50,
      "latency": 25.30,
      "packetLoss": 0,
      "activeConnections": 42,
      "cpuUsage": 2.15,
      "memoryUsage": 125.30
    }
  ]
}
```

---

## 🔧 Classe MonitoringEngine

```cpp
MonitoringEngine monitor;
monitor.setInterval(1000);        // 1 seconde
monitor.startMonitoring();

// ... continue monitoring ...

auto metrics = monitor.getMetrics(60);  // Dernières 60 secondes
for (const auto& m : metrics) {
    cout << "Bandwidth: " << m.bandwidth << " Mbps\n";
}

monitor.stopMonitoring();
```

---

## 📈 Classe DataExporter

```cpp
// Export monitoring data
std::vector<NetworkMetrics> metrics = monitor.getMetrics(60);
DataExporter::exportMonitoringToCsv("data.csv", metrics);
DataExporter::exportMonitoringToJson("data.json", metrics);

// Export ping results
PingStatistics stats = PingTest::performPing("google.com", 4);
DataExporter::exportToCsv("ping.csv", stats);
DataExporter::exportToJson("ping.json", stats);
```

---

## 🎯 Cas d'Utilisation Professionnels

✅ **ISP Troubleshooting** - Diagnostiquer problèmes de connexion client
✅ **Network Performance Analysis** - Analyser performances réseau
✅ **Compliance Reports** - Générer rapports pour conformité
✅ **SLA Monitoring** - Monitorer SLA temps réel
✅ **Network Planning** - Collecter données pour planification
✅ **Security Analysis** - Capturer et analyser trafic réseau

---

## 📊 Statistiques du Projet

- **Code Lines**: ~4500+ lignes C++ professionnel
- **Classes**: 13 classes avancées
- **Fonctions**: 80+ méthodes
- **Threads**: Multi-threading avec mutex
- **GUI**: Qt5 avec Charts
- **Exports**: CSV, JSON, TXT, PNG (graphiques)
- **Compilation**: < 2 minutes
- **Exécution**: < 100 MB RAM

---

## 🔐 Considérations de Sécurité

⚠️ **Administrator Privileges**: Certaines opérations nécessitent droits admin
⚠️ **Firewall**: Le port scanning peut être bloqué
⚠️ **Packet Capture**: Nécessite permissions système élevées
⚠️ **Privacy**: Ne pas capturer/exporter données sensibles

---

## 🚀 Améliorations Futures

- [ ] Support IPv6 complet
- [ ] Graphiques 3D temps réel
- [ ] WebSocket API pour accès distant
- [ ] Intégration Grafana
- [ ] Support Linux/macOS
- [ ] Machine Learning pour anomalies
- [ ] Dashboard web (React)
- [ ] Mobile app (Flutter)

---

## 📝 Licence

MIT License

---

## 👨‍💻 Auteur

**Développé par S.O.S INFO LUDO**

**Repository**: https://github.com/lerapeurdu62280-debug/NetDiag

---

## 🙏 Support

Pour aide, ouvrez une issue sur GitHub.
