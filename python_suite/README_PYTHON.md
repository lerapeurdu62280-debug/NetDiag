# NetworkDiagnostic Suite v2.0 - Python Edition

## 📦 **Suite Complète de PLUSIEURS Logiciels Python**

Une suite **professionnelle et complète** de 6+ applications Python compilées en **.EXE** pour Windows/Linux/macOS.

---

## 🎯 **APPLICATIONS INCLUSES**

### 1. **NetworkDiagnostic.py** (Application principale)
```
Fonctionnalités:
  ✅ IPv4/IPv6 complet
  ✅ Ping, DNS, Traceroute
  ✅ Monitoring temps réel
  ✅ Détection anomalies (ML)
  ✅ Graphiques 3D
  ✅ Export CSV/JSON
```
**Compilation:** `NetworkDiagnostic.exe` (~50 MB)

### 2. **PerformanceAnalyzer.py** (Analyse avancée)
```
Fonctionnalités:
  ✅ Graphiques en temps réel
  ✅ Analyse de tendances
  ✅ Prédictions (ML Linear Regression)
  ✅ Alertes automatiques
  ✅ Intégration Grafana
  ✅ Rapports détaillés
```
**Compilation:** `PerformanceAnalyzer.exe` (~45 MB)

### 3. **TrafficAnalyzer.py** (Analyse du trafic)
```
Fonctionnalités:
  ✅ Capture de paquets (Scapy)
  ✅ Analyse de protocoles
  ✅ Détection DDoS
  ✅ Classification ML du trafic
  ✅ Export PCAP/JSON
  ✅ Statistiques détaillées
```
**Compilation:** `TrafficAnalyzer.exe` (~60 MB)

### 4. **SecurityMonitor.py** (Sécurité réseau)
```
Fonctionnalités:
  ✅ Scan de ports multi-threaded
  ✅ Détection vulnérabilités
  ✅ Détection intrusions
  ✅ Threat intelligence
  ✅ Rapports sécurité
  ✅ Recommandations de sécurisation
```
**Compilation:** `SecurityMonitor.exe` (~40 MB)

### 5. **WebSocketAPI.py** (API en temps réel)
```
Fonctionnalités:
  ✅ Server WebSocket
  ✅ API REST
  ✅ Multi-clients
  ✅ Broadcast de métriques
  ✅ Commandes à distance
  ✅ Streaming temps réel
```
**Compilation:** `WebSocketAPI.exe` (~30 MB)

### 6. **DashboardWeb.py** (Dashboard navigateur)
```
Fonctionnalités:
  ✅ Interface web responsive
  ✅ Graphiques interactifs (Chart.js)
  ✅ Mise à jour temps réel
  ✅ Export de données
  ✅ Design moderne
  ✅ Accessible sur http://localhost:5000
```
**Compilation:** `DashboardWeb.exe` (~35 MB)

---

## 🚀 **INSTALLATION & COMPILATION**

### **Étape 1: Installer les dépendances**

```bash
cd python_suite
pip install -r requirements.txt
```

**Dépendances principales:**
- numpy, scipy, pandas
- matplotlib, seaborn
- scikit-learn (ML)
- scapy (packet capture)
- aiohttp, websockets
- flask, flask-cors
- pyinstaller

### **Étape 2: Compiler en .EXE**

#### Option A: Compilation complète (tous les EXE)
```bash
python build_exe.py
```

**Résultat:**
```
✓ SUCCESS: NetworkDiagnostic.exe
✓ SUCCESS: PerformanceAnalyzer.exe
✓ SUCCESS: TrafficAnalyzer.exe
✓ SUCCESS: SecurityMonitor.exe
✓ SUCCESS: WebSocketAPI.exe
✓ SUCCESS: DashboardWeb.exe

Total: 6/6 builds successful

[INFO] EXE files ready in ./dist/ directory
[INFO] Each .exe is standalone - no Python installation needed!
```

#### Option B: Compilation individuelle
```bash
# NetworkDiagnostic
pyinstaller --onefile --console --name=NetworkDiagnostic 1_NetworkDiagnostic.py

# PerformanceAnalyzer
pyinstaller --onefile --console --name=PerformanceAnalyzer 2_PerformanceAnalyzer.py

# TrafficAnalyzer
pyinstaller --onefile --console --name=TrafficAnalyzer 3_TrafficAnalyzer.py

# SecurityMonitor
pyinstaller --onefile --console --name=SecurityMonitor 4_SecurityMonitor.py

# WebSocketAPI
pyinstaller --onefile --console --name=WebSocketAPI WebSocketAPI.py

# DashboardWeb
pyinstaller --onefile --console --name=DashboardWeb DashboardWeb.py
```

### **Étape 3: Lancer les applications**

```bash
# Depuis ./dist/
./NetworkDiagnostic.exe
./PerformanceAnalyzer.exe
./TrafficAnalyzer.exe
./SecurityMonitor.exe
./WebSocketAPI.exe
./DashboardWeb.exe
```

---

## 💻 **UTILISATION DES APPLICATIONS**

### **1. NetworkDiagnostic.exe**

```
╔════════════════════════════════════════════════════╗
║  NetworkDiagnostic v2.0 - Advanced Python Edition  ║
║  IPv4/IPv6 • ML Anomalies • 3D Graphs • WebSocket  ║
╚════════════════════════════════════════════════════╝

[MENU]
1. Collect metrics (single shot)
2. Continuous monitoring (30 seconds)
3. Detect anomalies (ML)
4. Generate 3D plot
5. Export metrics
6. Exit

Select option: 1
Target (default 8.8.8.8): google.com

[METRICS]
  Latency: 25.34 ms
  Packet Loss: 0%
  IPv4: True, IPv6: True
  DNS Time: 12.45 ms
  Jitter: 2.34 ms
```

### **2. PerformanceAnalyzer.exe**

```
[MENU]
1. Add metric (manual)
2. Generate sample metrics
3. View statistics
4. Predict next hour (ML)
5. Generate report
6. Real-time graph demo
7. Exit

Select option: 4

{
  "predicted_avg_latency": 24.5,
  "predicted_max_latency": 35.8,
  "confidence": "moderate"
}
```

### **3. TrafficAnalyzer.exe**

```
[MENU]
1. Start packet capture
2. Detect DDoS attacks
3. Generate traffic report
4. Export to JSON
5. Exit

Select option: 1
Packet count (default 100): 50

[INFO] Starting packet capture...
[INFO] Captured 50 packets

Protocol Distribution:
  IPv4: 45 packets
  TCP: 30 packets
  UDP: 15 packets
```

### **4. SecurityMonitor.exe**

```
[MENU]
1. Scan ports
2. Detect vulnerabilities
3. Detect intrusions
4. Generate security report
5. Exit

Select option: 1
Target host (default 127.0.0.1): 192.168.1.1
End port (default 1024): 500

[OPEN] Port 22 (SSH)
[OPEN] Port 80 (HTTP)
[OPEN] Port 443 (HTTPS)

[SUCCESS] Found 3 open ports
```

### **5. WebSocketAPI.exe**

```
╔════════════════════════════════════════════════════╗
║  WebSocket API Server v2.0                         ║
║  Real-time Metrics • Remote Control                ║
╚════════════════════════════════════════════════════╝

[SERVER] WebSocket server started at ws://0.0.0.0:8765/ws
[CONNECT] Client connected
[CLIENTS] Total clients: 1
```

**Client JavaScript:**
```javascript
const ws = new WebSocket('ws://localhost:8765/ws');

ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log('Metrics:', data);
};

// Send command
ws.send(JSON.stringify({command: 'get_metrics'}));
```

### **6. DashboardWeb.exe**

```
[INFO] Starting web server...
[INFO] Open http://localhost:5000 in your browser
[INFO] Press Ctrl+C to stop
```

**Dashboard:**
- Affichage en temps réel
- 4 graphiques interactifs
- Métriques actuelles
- Export données

---

## 🎯 **AMÉLIORATIONS AVANCÉES**

### ✅ **IPv6 Complet**
- Support IPv4 et IPv6
- Détection automatique
- Ping dual-stack

### ✅ **Graphiques 3D Temps Réel**
- Matplotlib 3D
- Visualisation Latency/Loss/Bandwidth
- Export PNG

### ✅ **WebSocket API**
- Streaming temps réel
- Multi-clients
- Commandes à distance
- Async/await

### ✅ **Intégration Grafana**
- Push metrics
- Dashboards personnalisés
- Alertes intégrées

### ✅ **Support Cross-Platform**
- Windows (.exe)
- Linux (binary)
- macOS (binary)

### ✅ **Machine Learning Avancé**
- Détection anomalies (Isolation Forest)
- Prédictions (Linear Regression)
- Classification trafic (Random Forest)
- Alertes intelligentes

### ✅ **Dashboard Web Moderne**
- Interface responsive
- Charts.js interactifs
- REST API
- Export JSON

### ✅ **Fonctionnalités Supplémentaires**
- Multi-threading
- Historique 3600 points
- Export CSV/JSON/PCAP
- Rapports détaillés
- Alertes temps réel

---

## 📊 **STATISTIQUES SUITE COMPLÈTE**

| Métrique | Valeur |
|----------|--------|
| **Nombre d'apps** | 6 applications Python |
| **Lignes de code** | ~2000+ lignes Python |
| **Fichiers .EXE** | 6 exécutables standalone |
| **Taille totale** | ~280 MB |
| **Classes** | 15+ classes avancées |
| **Fonctionnalités** | 50+ fonctionnalités |
| **ML Models** | 4 modèles ML |
| **Formats export** | 5+ formats |
| **Threads** | Multi-threaded |
| **État** | 🟢 PRODUCTION READY |

---

## 🔧 **DÉVELOPPEMENT PERSONNALISÉ**

### Ajouter une nouvelle application:

```python
# 5_CustomApp.py
import json

class CustomAnalyzer:
    def __init__(self):
        self.data = []
    
    def analyze(self):
        return self.data

def main():
    app = CustomAnalyzer()
    # Implementation

if __name__ == '__main__':
    main()
```

Puis compiler:
```bash
pyinstaller --onefile --console --name=CustomApp 5_CustomApp.py
```

---

## 🆘 **DÉPANNAGE**

### PyInstaller ne trouve pas les modules
```bash
pip install --upgrade pyinstaller
pip install pyinstaller-hooks-contrib
```

### Scapy pour capture de paquets
```bash
# Windows: Installer Npcap
# https://nmap.org/npcap/

# Linux:
sudo apt-get install python3-scapy

# macOS:
brew install libdnet libpcap
pip install scapy
```

### Performance
- Réduire `maxlen` des deques pour moins de RAM
- Utiliser `--onefile` pour portabilité
- Compiler en Release pour vitesse

---

## 📦 **DISTRIBUTION**

### Créer un installateur Windows
```bash
# Installer NSIS
choco install nsis

# Créer script d'installation
# (Voir NSIS documentation)
```

### Distribution Linux
```bash
# Créer AppImage
pip install appimage-builder

# Ou distribuer le binary directly
chmod +x NetworkDiagnostic
```

---

## 🌟 **PROCHAINES ÉTAPES**

1. **Compiler toutes les applications**
   ```bash
   python build_exe.py
   ```

2. **Tester chaque application**
   ```bash
   dist/NetworkDiagnostic.exe
   dist/PerformanceAnalyzer.exe
   # etc...
   ```

3. **Configurer Grafana** (optionnel)
   - Créer datasource
   - Importer dashboards
   - Configurer alertes

4. **Déployer**
   - Distribuer .EXE
   - Documenter pour utilisateurs
   - Support & mises à jour

---

## 📞 **Support**

Pour aide, consultez:
- Documentation officielle PyInstaller: https://pyinstaller.readthedocs.io
- Scapy documentation: https://scapy.readthedocs.io
- Sklearn documentation: https://scikit-learn.org

---

## 📝 **Licence**

MIT License - Libre d'utilisation et modification

---

**Suite Complète & Professionnelle Prête à l'Emploi! 🚀**
