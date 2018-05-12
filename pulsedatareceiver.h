#ifndef PULSEDATARECEIVER_H
#define PULSEDATARECEIVER_H

#include <QObject>
#include <QIODevice>

#include <QtCharts/QChartGlobal>
#include <QtCharts/QXYSeries>
#include <QtCharts/QValueAxis>
#include <QVector>
#include <QDateTime>
#include <QDebug>


QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

struct InputDataStruct{
    int bmp;
    int raw_value;
};

class PulseDataReceiver : public QIODevice
{
    Q_OBJECT
private:
    QXYSeries *m_series;
    QValueAxis *m_axis;
    bool newSer;
    int index;
    float viewing_timespan;
    int offest; //samples
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);
public:
    explicit PulseDataReceiver(QXYSeries * series, QObject *parent = 0);
    void newSeries(QXYSeries * series, QValueAxis *x_axis, float timespan);
};

#endif // PULSEDATARECEIVER_H
