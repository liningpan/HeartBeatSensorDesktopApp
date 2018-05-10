#ifndef PULSEDATARECEIVER_H
#define PULSEDATARECEIVER_H

#include <QObject>
#include <QIODevice>

#include <QtCharts/QChartGlobal>
#include <QtCharts/QXYSeries>
#include <QVector>
#include <QDateTime>
#include <QDebug>


QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


class PulseDataReceiver : public QIODevice
{
    Q_OBJECT
private:
    QXYSeries *m_series;
    bool newSer;
    int index;
    float interval;
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);
public:
    explicit PulseDataReceiver(QXYSeries * series, QObject *parent = 0);
    void newSeries(QXYSeries * series, float inter);
};

#endif // PULSEDATARECEIVER_H
