# NetDiag - Network Diagnostic Tool

## 📋 Description

**NetDiag** est un outil complet de diagnostic réseau pour Windows, écrit en C++. Il permet de tester et diagnostiquer votre connexion réseau avec un ensemble de fonctionnalités professionnelles.

### Fonctionnalités principales

✅ **Network Information** - Affiche les adaptateurs réseau, IP, DNS, etc.
✅ **Ping Test** - Teste la latence et la perte de paquets
✅ **DNS Lookup** - Résolution DNS forward et reverse
✅ **Traceroute** - Trace l'itinéraire du réseau jusqu'à une cible
✅ **Port Scan** - Analyse les ports TCP ouverts
✅ **Speed Test** - Mesure la vitesse de téléchargement/upload
✅ **System Information** - Affiche les infos système

---

## 🔧 Prérequis

- **Windows 7 ou supérieur**
- **Visual Studio 2019** ou **MinGW** (pour compiler)
- **CMake 3.10+**
- **C++17 ou supérieur**

---

## 📦 Installation

### 1. Cloner le repository

```bash
git clone https://github.com/lerapeurdu62280-debug/NetDiag.git
cd NetDiag
```

### 2. Compiler avec Visual Studio

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
.\Release\NetDiag.exe
```

### 3. Compiler avec MinGW

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
.\NetDiag.exe
```

---

## 🎮 Utilisation

Lancez l'exécutable et suivez le menu interactif :

```
╔════════════════════════════════════════╗
║           MAIN MENU                    ║
╚════════════════════════════════════════╝

1. Network Information
2. Ping Test
3. DNS Lookup
4. Traceroute
5. Port Scan
6. Speed Test
7. System Information
0. Exit
```

### Exemples d'utilisation

#### Ping Test
```
Enter target (domain or IP): google.com
Pinging google.com...
Reply from google.com: time=25ms
```

#### DNS Lookup
```
1. Forward Lookup (Domain -> IP)
Enter domain name: github.com
→ Résout github.com en adresses IP
```

#### Port Scan
```
Enter target: localhost
Enter starting port: 1
Enter ending port: 65535
→ Scanne tous les ports TCP ouverts
```

---

## 📂 Structure du projet

```
NetDiag/
├── src/
│   ├── main.cpp                    # Point d'entrée
│   ├── Utils.h/cpp                 # Utilitaires
│   ├── NetworkInfo.h/cpp           # Info réseau
│   ├── PingTest.h/cpp              # Tests ping
│   ├── DNSTest.h/cpp               # Résolution DNS
│   ├── TracerouteTest.h/cpp        # Traceroute
│   ├── PortScanTest.h/cpp          # Scanner de ports
│   └── SpeedTest.h/cpp             # Tests de vitesse
├── CMakeLists.txt
├── README.md
└── LICENSE
```

---

## 🔗 Dépendances Windows

- **Winsock2** - Socket réseau
- **Iphlpapi** - IP Helper API
- **Icmpapi** - ICMP API (Ping)
- **Wininet** - Internet API

---

## 📊 Statistiques du Code

- **~1800 lignes** de C++ pur
- **8 classes** principales
- **40+ fonctions** utilitaires
- **Compilable immédiatement**

---

## 🚀 Améliorations futures

- [ ] Interface GUI avec Qt
- [ ] Export des résultats en CSV/JSON
- [ ] Monitoring en temps réel
- [ ] Graphiques de performance
- [ ] Capture de paquets

---

## 📝 Licence

MIT License - Voir le fichier LICENSE pour plus de détails

---

## 👨‍💻 Auteur

**Développé par S.O.S INFO LUDO**

---

## 🙏 Support

Pour toute question ou bug, ouvrez une issue sur GitHub.

**Repository** : https://github.com/lerapeurdu62280-debug/NetDiag