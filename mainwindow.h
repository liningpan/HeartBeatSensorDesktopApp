#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QList>
#include "pulsedevice.h"

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QVXYModelMapper>
#include <QtCharts>
#include <QTimer>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
class QXYSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

#include "pulsedatareceiver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateBpm(int bpm);

private:
    Ui::MainWindow *ui;
    QList<QSerialPortInfo> serialPortList;
    PulseDevice* pd;
    QTimer* timer;
    PulseDataReceiver* pdr;
    QXYSeries* currentSeries;
    QValueAxis* x;
    QValueAxis* y;
    QChart* m_chart;
    bool connected;
    bool start;
    void setSampleParameterDisable(bool);
    void createNewSeries();
private slots:
    void on_updateButton_clicked();
    void on_connectButton_clicked();
    void on_startButton_clicked();
    void on_durationBox_valueChanged(int);
    void releaseStart();
public slots:
    void finishRead();
};

#endif // MAINWINDOW_H
