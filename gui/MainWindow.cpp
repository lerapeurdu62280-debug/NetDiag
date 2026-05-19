#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QGroupBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), monitor(nullptr), packetCapture(nullptr) {
    setWindowTitle("NetDiag PRO - Network Diagnostic Tool");
    setGeometry(100, 100, 1200, 800);
    
    setupUI();
    createChart();
    createConnections();
}

MainWindow::~MainWindow() {
    if (monitor) delete monitor;
    if (packetCapture) delete packetCapture;
}

void MainWindow::setupUI() {
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create main layout
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    // Left panel - Controls
    QVBoxLayout* leftLayout = new QVBoxLayout();
    
    QGroupBox* testGroup = new QGroupBox("Network Tests", this);
    QVBoxLayout* testLayout = new QVBoxLayout(testGroup);
    
    networkInfoBtn = new QPushButton("Network Information", this);
    pingBtn = new QPushButton("Ping Test", this);
    dnsBtn = new QPushButton("DNS Lookup", this);
    monitoringBtn = new QPushButton("Start Monitoring", this);
    packetCaptureBtn = new QPushButton("Packet Capture", this);
    exportBtn = new QPushButton("Export Results", this);
    
    testLayout->addWidget(networkInfoBtn);
    testLayout->addWidget(pingBtn);
    testLayout->addWidget(dnsBtn);
    testLayout->addWidget(monitoringBtn);
    testLayout->addWidget(packetCaptureBtn);
    testLayout->addWidget(exportBtn);
    testLayout->addStretch();
    
    leftLayout->addWidget(testGroup);
    
    // Status display
    QGroupBox* statusGroup = new QGroupBox("Current Status", this);
    QVBoxLayout* statusLayout = new QVBoxLayout(statusGroup);
    
    bandwidthLabel = new QLabel("Bandwidth: -- Mbps", this);
    latencyLabel = new QLabel("Latency: -- ms", this);
    packetLossLabel = new QLabel("Packet Loss: -- %", this);
    statusLabel = new QLabel("Ready", this);
    
    statusLayout->addWidget(bandwidthLabel);
    statusLayout->addWidget(latencyLabel);
    statusLayout->addWidget(packetLossLabel);
    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();
    
    leftLayout->addWidget(statusGroup);
    
    // Right panel - Chart
    QVBoxLayout* rightLayout = new QVBoxLayout();
    chartView = new QtCharts::QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(chartView);
    
    // Add layouts to main
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 2);
}

void MainWindow::createChart() {
    chart = new QtCharts::QChart();
    chart->setTitle("Real-Time Network Metrics");
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    
    bandwidthSeries = new QtCharts::QSplineSeries();
    bandwidthSeries->setName("Bandwidth (Mbps)");
    
    latencySeries = new QtCharts::QSplineSeries();
    latencySeries->setName("Latency (ms)");
    
    chart->addSeries(bandwidthSeries);
    chart->addSeries(latencySeries);
    
    // Add axes
    QtCharts::QValueAxis* axisX = new QtCharts::QValueAxis();
    QtCharts::QValueAxis* axisY = new QtCharts::QValueAxis();
    
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    
    bandwidthSeries->attachAxis(axisX);
    bandwidthSeries->attachAxis(axisY);
    latencySeries->attachAxis(axisX);
    latencySeries->attachAxis(axisY);
    
    chartView->setChart(chart);
}

void MainWindow::createConnections() {
    connect(networkInfoBtn, &QPushButton::clicked, this, &MainWindow::onNetworkInfoClicked);
    connect(pingBtn, &QPushButton::clicked, this, &MainWindow::onPingTestClicked);
    connect(dnsBtn, &QPushButton::clicked, this, &MainWindow::onDNSTestClicked);
    connect(monitoringBtn, &QPushButton::clicked, this, &MainWindow::onMonitoringStarted);
    connect(packetCaptureBtn, &QPushButton::clicked, this, &MainWindow::onPacketCaptureClicked);
    connect(exportBtn, &QPushButton::clicked, this, &MainWindow::onExportClicked);
}

void MainWindow::onNetworkInfoClicked() {
    statusLabel->setText("Loading network information...");
    // Implementation here
}

void MainWindow::onPingTestClicked() {
    statusLabel->setText("Performing ping test...");
    // Implementation here
}

void MainWindow::onDNSTestClicked() {
    statusLabel->setText("Performing DNS lookup...");
    // Implementation here
}

void MainWindow::onMonitoringStarted() {
    if (!monitor) {
        monitor = new MonitoringEngine();
        monitor->startMonitoring();
        statusLabel->setText("Monitoring started");
        monitoringBtn->setText("Stop Monitoring");
    } else {
        monitor->stopMonitoring();
        delete monitor;
        monitor = nullptr;
        statusLabel->setText("Monitoring stopped");
        monitoringBtn->setText("Start Monitoring");
    }
}

void MainWindow::onMonitoringUpdate() {
    if (monitor && monitor->isRunning()) {
        NetworkMetrics metrics = monitor->getCurrentMetrics();
        updateMonitoringDisplay(metrics);
        updateChart();
    }
}

void MainWindow::onPacketCaptureClicked() {
    statusLabel->setText("Capturing packets...");
    if (!packetCapture) {
        packetCapture = new PacketCapture();
        packetCapture->startCapture("", 100);
    }
}

void MainWindow::onExportClicked() {
    statusLabel->setText("Exporting results...");
    // Implementation here
}

void MainWindow::updateChart() {
    // Update chart with new data
}

void MainWindow::updateMonitoringDisplay(const NetworkMetrics& metrics) {
    bandwidthLabel->setText(QString("Bandwidth: %1 Mbps").arg(metrics.bandwidth, 0, 'f', 2));
    latencyLabel->setText(QString("Latency: %1 ms").arg(metrics.latency, 0, 'f', 2));
    packetLossLabel->setText(QString("Packet Loss: %1 %").arg(metrics.packetLoss));
}
