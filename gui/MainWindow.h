#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include "../src/MonitoringEngine.h"
#include "../src/PacketCapture.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onNetworkInfoClicked();
    void onPingTestClicked();
    void onDNSTestClicked();
    void onMonitoringStarted();
    void onMonitoringUpdate();
    void onPacketCaptureClicked();
    void onExportClicked();
    void updateChart();

private:
    void setupUI();
    void createConnections();
    void createChart();
    void updateMonitoringDisplay(const NetworkMetrics& metrics);
    
    QLabel* statusLabel;
    QLabel* bandwidthLabel;
    QLabel* latencyLabel;
    QLabel* packetLossLabel;
    
    QPushButton* networkInfoBtn;
    QPushButton* pingBtn;
    QPushButton* dnsBtn;
    QPushButton* monitoringBtn;
    QPushButton* packetCaptureBtn;
    QPushButton* exportBtn;
    
    QtCharts::QChartView* chartView;
    QtCharts::QChart* chart;
    QtCharts::QSplineSeries* bandwidthSeries;
    QtCharts::QSplineSeries* latencySeries;
    
    MonitoringEngine* monitor;
    PacketCapture* packetCapture;
};
